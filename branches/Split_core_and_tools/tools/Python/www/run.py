#!/usr/bin/env python

# setup flask.ext for old versions of flask (<8)
import flaskext_compat
flaskext_compat.activate()


from flask import *
from uuid import uuid4 as uuid
import sys

from app import app, db, db_session, SessionMaker, ModelBase
from models import Job, Simulation, SimRun, Param, ParamValue, ParamDefault, User

from util import skip, templated, with_nonce, get_nonce, check_nonce, \
     authenticated, authenticate, get_session, one_or_none, new_id


def convert_type(default, str_value):
    # tested types: str and float
    return type(default.value)(str_value)


@app.route('/')
def index():
    jobid = new_id()
    return redirect(url_for('configure', jobid=jobid))

def get_sims():
    return Simulation.query.order_by('simulation.name').all()


@app.route('/login/<path:next>', methods=['GET'])
@templated()
def login(next):
    return dict(next = next)


@app.route('/login/<path:next>', methods=['POST'])
def loginPOST(next):
    form = request.form
    usernm = form.get('username', '')
    passwd = form.get('password', '')
    # check information
    if not authenticate(usernm, passwd):
        return redirect(url_for('login', next=next))
    # All ok, register user
    session = get_session()
    session['user'] = usernm
    resp = redirect(next)
    app.save_session(session, resp)
    return resp


@app.route('/job/<jobid>', methods=['GET'])
@with_nonce()
@authenticated()
@templated()
def configure(jobid):
    job = Job.query.get(jobid)
    sims = get_sims()
    return dict(sims = sims, job=job, jobid=jobid, nonce=get_nonce())


@app.route('/job/update/<jobid>', methods=['POST'])
@authenticated()
@check_nonce()
def configurePOST(jobid):
    oks    = []
    errors = []  # all ok
    def ok(name, old, f):
        ''' check parameter and update either oks or errors '''
        try:
            v = f()
            oks.extend([name])
            return v
        except:
            errors.extend([name])
            return old

    form = request.form
    sim = Simulation.query.filter_by(name=request.form['sim']).one()

    # defualts
    seed    = 0
    samples = 100000

    # lookup job
    job = Job.query.get(jobid)
    if job is None:
        # create job
        # TODO: check types of seed and samples
        job = Job(id=jobid, seed=seed, samples=samples, sim=sim)
        db_session.add(job)

    seed    = ok("seed",    seed,    lambda : abs(int(form['seed'])))
    samples = ok("samples", samples, lambda : abs(int(form['samples'])))

    # update job
    job.seed = seed
    job.samples = samples
    job.sim_id = sim.id

    # commit job
    db_session.commit()

    # insert / update params
    for name in skip(('__nonce', 'sim', 'seed', 'samples'), form):
        str_value = form[name]
        param  = Param.query.filter_by(name=name).one()
        paramd = ParamDefault.query.filter_by(param_id=param.id, sim_id=sim.id).one()

        # lookup parameter value
        oldP = ParamValue.query.filter_by(job_id=job.id, param_id=param.id)
        oldP = one_or_none(oldP)

        # pick parameter value if present or use default
        old = oldP is None and paramd.value or oldP.value

        cvalue = ok(name, old, lambda : convert_type(paramd, str_value))

        valueQ = ParamValue.query.filter_by(job_id=job.id, param_id=param.id)
        pvalue = one_or_none(valueQ)
        if pvalue is None:
            # create parameter value
            pvalue = ParamValue(param=param, job=job, value=cvalue)
            db_session.add(pvalue)
        # commit parameter value
        pvalue.value = cvalue
        db_session.commit()

    return jsonify(errors=errors, oks=oks)


@app.route('/sim/<jobid>', methods=['POST'])
@authenticated()
@check_nonce()
def simulatePOST(jobid):
    ''' Create simulation job for the worker '''
    job = Job.query.get(jobid)
    sim = Simulation.query.get(job.sim_id)
    # treat seed and samples specially
    params = { "_seed": job.seed,
               "_samples": job.samples }
    # filter params by what the simulation expects (needed!)
    valid = set(pd.param.name for pd in sim.params)
    params.update(dict(
        (p.param.name, p.value) for p in job.params
        if p.param.name in valid))
    # create simulation run (for the worker to compute)
    run = SimRun(job=job, sim=sim, params=params)
    db_session.add(run)
    db_session.commit()
    # send user to status page
    return redirect(url_for('status', jobid=job.id, runid=run.id))


@app.route('/sim/status/<jobid>/<runid>', methods=['GET'])
@templated()
def status(jobid, runid):
    return dict(runid = runid)


if __name__ == '__main__':
    if '--init' in sys.argv[1:]:
        print 'Creating database..'
        db.init_app(app)
        ModelBase.metadata.create_all(bind=db.engine)
        sys.exit(0)
    app.run(debug=True)
