/*******************************************************************************
*
* McStas, neutron ray-tracing package
*         Copyright (C) 1997-2006, All rights reserved
*         Risoe National Laboratory, Roskilde, Denmark
*         Institut Laue Langevin, Grenoble, France
*
* Kernel: cogen.c
*
* %Identification
* Written by: K.N.
* Date: Aug  20, 1997
* Origin: Risoe
* Release: McStas 1.6
* Version: $Revision: 1.89 $
*
* Code generation from instrument definition.
*
* $Log: cogen.c,v $
* Revision 1.89  2009/08/13 13:43:50  farhi
* Cosmetics on automatic comments. Also added a note about GCC4 warnings
* when using definition parameters. We should probably move most of them
* to SETTING parameters in comps.
*
* Revision 1.88  2009/08/13 11:39:40  pkwi
* Commits from Morten Siebuhr after EK and PW code review.
*
* A few test instruments have prolonged compile time, but more things are in the
* pipe that should remedy this.
*
*   Changes:
*
*        * Compute-intensive #defines -> functions
*        * Change memcpy() to just copy data for small values - somewhat
*          faster.
*        * Correct a bug in coords_mirror(), as previously discussed on
*          this list.
*        * Remove unneeded parameters to mccoordschange() - improves speed
*          slightly.
*
*   The changes are mostly in mcstas-r.c, mcstas-r.h and two lines in
*   cogen.c.
*
* Revision 1.87  2009/06/12 13:48:32  farhi
* mcstas-r: nan and inf detection in PROP and detector output
* mcstas-r: MPI writing files when p0==0. Now divide by MPI_nodes.
*
* Revision 1.86  2009/04/16 13:11:03  farhi
* Corrected bug for SPLIT on 'passive' components.
*
* Revision 1.85  2009/02/20 16:17:54  farhi
* Fixed warnings and a few bugs detected with GCC 4.3.
*
* Revision 1.84  2009/02/20 13:52:27  farhi
* Force type of 'string' to (char*) in generated code for DEFINITION
* PARAMETERS
*
* Revision 1.83  2008/10/21 15:20:33  farhi
* Info SPLIT indicates name of instrument
*
* Revision 1.82  2008/08/29 15:37:14  farhi
* cogen: SPLIT low stat indicates number of events
* example: fixed %example reference values, and some zero divergence
* sources
* optics: added comments, fixed bug in recent DiskChopper
* added Test_Fermi: ok, but Guide_gravity in rotating mode fails
*
* Revision 1.81  2008/06/13 09:16:47  pkwi
* Reverting to next-to-last version of cogen.c
*
* Revision 1.79  2008/02/09 22:26:26  farhi
* Major contrib for clusters/multi-core: OpenMP support
* 	try ./configure --with-cc=gcc4.2 or icc
* then mcrun --threads ...
* Also tidy-up configure. Made relevant changes to mcrun/mcgui to enable OpenMP
* Updated install-doc accordingly
*
* Revision 1.78  2007/12/11 14:49:32  farhi
* WHEN now disabled EXTEND
* Added IN22 examples
*
* Revision 1.77  2007/10/29 18:41:18  farhi
* Fixed GROUP issue that came in after SPLIT insertion in grammar
*
* Revision 1.76  2007/07/30 13:43:07  farhi
* Instrument parameters may be specified more than once (specially when using %include "instr", e.g. Lambda in all instr). Recurrent parameters are
* skipped, only the first def is used and a warning is issued.
*
* Revision 1.75  2007/06/25 12:37:29  pkwi
* Split bug corrected!
*
* Split_counter is smaller than rep count also if there is no split! - Made
* previously ABSORB'ed neutrons go to the split position.
*
* mcstas-1.11 is now one step closer to release...
*
* Revision 1.74  2007/04/03 13:16:58  farhi
* Fixed potential infinite loop when using SPLIT
*
* Revision 1.73  2007/04/02 12:11:31  farhi
* changed ENHANCE keyword to SPLIT
*
* Revision 1.72  2007/03/14 15:33:22  farhi
* records N,p,p2 for each comp, for profiling in Progress_bar
*
* Revision 1.71  2007/03/12 14:12:16  farhi
* SPLIT COMPONENT grammar: new rule for yacc. Code generation and cleaner GROUP handling.
*
* Revision 1.70  2007/03/06 09:39:09  farhi
* NeXus default output is now "5 zip". Then NEXUS keyword is purely optional.
*
* Revision 1.69  2007/03/05 19:02:53  farhi
* NEXUS support now works as MPI. NEXUS keyword is optional and only -DUSE_NEXUS is required. All instruments may then export in NEXUS if McStas
* has been installed with --with-nexus
*
* Revision 1.68  2007/03/02 14:27:17  farhi
* NEXUS grammar is now simplified. a single string may follow the keyword.
* NEXUS "5 ZIP" is recommended.
*
* Revision 1.67  2007/02/09 13:19:15  farhi
* When no NEXUS keyword in instrument and NeXus output requested, displays
* an error/tip to add keyword after INITIALIZE
*
* Revision 1.66  2007/01/26 16:23:22  farhi
* NeXus final integration (mcplot, mcgui, mcrun).
* Only mcgui initiate mcstas.nxs as default output file, whereas
* simulation may use instr_time.nxs
*
* Revision 1.65  2007/01/21 15:43:04  farhi
* NeXus support. Draft version (functional). To be tuned.
*
* Revision 1.64  2006/12/19 15:11:56  farhi
* Restored basic threading support without mutexes. All is now in mcstas-r.c
*
* Revision 1.63  2006/08/29 15:57:07  farhi
* improved threads efficiency by using mutexes
*
* Revision 1.62  2006/08/28 10:34:25  pchr
* In cogen.c - implementation of dashed line for mcdisplay.
*
* Added component Pol_simpleBfield handeling magnetic fields and spin precession.
*
* Test instruments for different types of usage supplied.
*
* Revision 1.61  2006/07/06 08:58:36  pchr
* 'Added rectangle and box drawing modes.
*
* Revision 1.60  2006/04/19 13:06:25  farhi
* * Updated Release, Version and Origin fields in headers
* * Improved setversion to update all McStasx.y occurencies into current release
* * Added 'string' type for DEFINITION parameters to be handled as this type so that auto-quoting occurs in mcgui
* * Added possibility to save log of the session to a file (appended) in mcgui
* * Made Scilab use either TCL_EvalStr or TK_EvalStr
*
* Revision 1.59  2006/04/06 08:46:20  farhi
* NEW GRAMMAR: JUMP WHEN ITERATE COPY ...
* need doc in TeX now...
*
* Revision 1.58  2005/11/02 09:18:38  farhi
* More tolerant about DEFINITION parameter values, enabling table init as for PowderN
*
* Revision 1.57  2005/09/15 10:46:10  farhi
* Added mccompcurtype to be e.g. the component type/class being used.
* Not used yet, but will be with NeXus support and probably elsewere
*
* Revision 1.56  2005/07/27 11:29:41  farhi
* missing arg in printf
*
* Revision 1.55  2005/07/18 14:43:05  farhi
* Now gives a warning message per component for 'computational absorbs'
*
* Revision 1.54  2005/07/06 08:24:43  farhi
* Moved again SHAREd blocks at the very begining of instrument C code
* so that no name clash can occur.
*
* Revision 1.53  2005/07/05 12:18:00  farhi
* Moved SHAREs defore definition/setting parameter comp definition
* to avoid name clash. Was reason for failure of updated read_table-lib
*
* Revision 1.52  2005/06/20 13:32:45  farhi
* Only display mcstas-r.* embed files with --verbose
*
* Revision 1.51  2005/06/20 09:16:48  farhi
* Corrected mcScattered definition (char -> MCNUM)
*
* Revision 1.50  2005/06/20 09:02:10  farhi
* Some more info in mcstas --verbose mode.
*
* Revision 1.49  2005/06/20 08:01:58  farhi
* Install ABSORB counter for run-time PROP macros.
* Report at end of simulation if needed.
*
* Revision 1.48  2005/05/31 13:26:16  farhi
* Make possible to change setting parameter values in the INITIALIZE section.
* It is thus easier to auto-configure components in their INIT code, depending
* on some tests and checks...
* Other sections use local copies of the setting parameters, e.g. TRACE, EXTEND
* FINALLY, SAVE, ...
*
* Revision 1.47  2005/02/17 15:51:02  farhi
* Added a neutron event per component, so that amessage is displayed when
* no neutron reaches the COMP in FINALLY. Requested by R. Cubitt
*
* Revision 1.46  2004/11/30 16:09:56  farhi
* Uses SIG_MESSAGE macro defined as strcpy(mcsigmessage...) when signals are used
* or ignored when NOSIGNALS
*
* Revision 1.45  2004/11/29 14:30:52  farhi
* Defines a component name as instrument name for usage of mcstas-r functions
* and macros in FINALLY and SAVE (e.g. DETECTOR_OUT...)
*
* Revision 1.44  2004/09/10 15:09:56  farhi
* Use same macro symbols for mcstas kernel and run-time for code uniformity
*
* Revision 1.43  2004/09/03 13:43:29  farhi
* Removed duplicated Instr:FINALLY code (in SAVE and FINALLY). May cause SEGV.
*
* Revision 1.42  2003/10/06 14:59:14  farhi
* Also insert component index in automatic source comments
*
* Revision 1.41  2003/09/05 08:59:05  farhi
* added INSTRUMENT parameter default value grammar
* mcinputtable now has also default values
* mcreadpar now uses default values if parameter not given
* extended instr_formal parameter struct
* extended mcinputtable structure type
*
* Revision 1.40  2003/08/12 13:32:25  farhi
* Add generation date/time in C code header
*
* Revision 1.39  2003/02/11 12:28:45  farhi
* Variouxs bug fixes after tests in the lib directory
* mcstas_r  : disable output with --no-out.. flag. Fix 1D McStas output
* read_table:corrected MC_SYS_DIR -> MCSTAS define
* monitor_nd-lib: fix Log(signal) log(coord)
* HOPG.trm: reduce 4000 points -> 400 which is enough and faster to resample
* Progress_bar: precent -> percent parameter
* CS: ----------------------------------------------------------------------
*
* Revision 1.24 2002/09/17 10:34:45 ef
* added comp setting parameter types
*
* $Id: cogen.c,v 1.89 2009/08/13 13:43:50 farhi Exp $
*
*******************************************************************************/

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include "mcstas.h"


/*******************************************************************************
* Some general comments on code generation.
*
* Code is output in the form of strings using the following functions:
*   cout();                        (one line at a time)
*   coutf();                       (with printf-style formatting)
*
* The type of numbers used in the generated code is given by the macro MCNUM
* (defined in mcstas-r.h).
*
* All generated identifiers are prefixed with the string ID_PRE, to make name
* clashes less likely to occur. Currently, for simplicity, we output modified
* names directly, eg.
*
*   cout("void " ID_PRE "init(void);");
*
* But to make a later transition to a better name generation scheme possible,
* it is important to use the ID_PRE macro everywhere identifiers are
* generated.
*
* After the ID_PRE prefix a few letters occur in generated names to
* distinguish different kinds of identifiers (instrument parameters,
* component definition parameters, internal temporary variables and so on).
* Care must be takes to choose these letters such that no name clashes will
* occur with names generated by other parts of the code or generated from
* user symbols.
*
* Finally, names generated from user symbols are generally chosen to match
* the originals as closely as possible to make the generated code more
* readable (for debugging purposes).
*
* The following is a list of the identifiers generated in the output. The
* ID_PRE prefix is denoted by ##. The first column gives the identifier as it
* appears in the generated code, the second explains the origin of the
* identifier in the instrument definition source (if any).
*
* ##ip<PAR>        From instrument parameter <PAR>.
* ##init           Function containing initialization code.
* ##inputtable     Table of instrument parameters.
* ##NUMIPAR        Macro giving the number of instrument parameters.
* ##numipar        Global variable with the value of ##NUMIPAR.
* ##c<C>_<P>       From definition or setting parameter <P> in component
*                  instance <C>.
* ##posa<COMP>     Absolute position of coordinate system of <COMP>.
* ##posr<COMP>     Position of <COMP> relative to previous component.
* ##rota<COMP>     Absolute rotation.
* ##rotr<COMP>     Relative rotation.
* ##tc1            Temporary variable used to compute transformations.
* ##tc2
* ##tr1
* ##compcurname
* ##compcurtype
* ##compcurindex
* ##absorb          label for ABSORB (goto)
* ##NCounter        Incremented each time a neutron is entering the component
* ##AbsorbProp      single counter for removed events in PROP calls
* ##comp_storein    Positions of neutron entering each comp (loc. coords)
* ##Group<GROUP>    Flag true when in an active group
* ##sig_message     Message for the signal handler (debug/trace, sim status)
* ##JumpCounter     iteration counter for JUMP
*******************************************************************************/



/*******************************************************************************
* Generation of declarations.
*
* The following declarations are generated (## denotes the value ID_PRE):
* 1. Header file #include - "mcstas-r.h" for declarations for the
*    mcstas runtime.
* 2. Declarations of global variables to hold the values of the instrument
*    parameters. For example, for an instrument parameter OMM, the
*    declaration "MCNUM ##ipOMM;" is generated.
* 3. Declaration of a table ##inputtable containing the list of instrument
*    parameters. For each parameter, the name, a pointer to the
*    corresponding global variable, and the type (double, int,
*    string) is given. The macro ##NUMIPAR gives the number of
*    entries in the table and is also found as the value of the
*    variable ##numipar; in addition, the table is terminated by two
*    NULLs. This table is used to read the instrument parameters from
*    the user or from another program such as TASCOM.
* 4. User declarations copied verbatim from the instrument definition file.
* 5. Declarations for the component parameters. This uses #define for
*    definition parameters and global variables for setting parameters.
* X. User declarations from component definitions.
* X. Declarations of variables for coordinate system transformations.
* X. Declaration of variables for neutron state.
* X. Function prototypes.
*******************************************************************************/


/* Functions for outputting code. */

/* Handle for output file. */
static FILE *output_handle = NULL;/* Handle for output file. */
static int num_next_output_line = 1; /* Line number for next output line. */
static char *quoted_output_file_name = NULL; /* str_quote()'ed name
                                                of output file. */

/* Convert instrument formal parameter type numbers to their enum name. */
char *instr_formal_type_names[] =
  { "instr_type_double", "instr_type_int", "instr_type_string" };

char *instr_formal_type_names_real[] =
  { "MCNUM", "int", "char*", "char"}; /* the last one is for static char allocation */

/*******************************************************************************
* Output a line of code
* Assumes that the output does not contain newlines.
*******************************************************************************/
static void
cout(char *s)
{
  fprintf(output_handle, "%s\n", s);
  num_next_output_line++;
}

/*******************************************************************************
* Output a line of code using printf-style format string.
* Assumes that the output does not contain newlines.
*******************************************************************************/
static void
coutf(char *format, ...)
{
  va_list ap;

  va_start(ap, format);
  vfprintf(output_handle, format, ap);
  va_end(ap);
  fprintf(output_handle, "\n");
  num_next_output_line++;
}

/*******************************************************************************
* Output #line directive to handle code coming from a different file.
* The filename is assumed to be already properly quoted for special chars.
*******************************************************************************/
static void
code_set_source(char *filename, int linenum)
{
  if(linenum > 0)
    coutf("#line %d \"%s\"", linenum, filename);
}

/*******************************************************************************
* Output #line directive to reset back to the generated output C file.
*******************************************************************************/
static void
code_reset_source(void)
{
  /* Note: the number after #line refers to the line AFTER the directive. */
  coutf("#line %d \"%s\"", num_next_output_line + 1, quoted_output_file_name);
}


static void
codeblock_out(struct code_block *code)
{
  List_handle liter;                /* For list iteration. */
  char *line;                        /* Single code line. */

  if(list_len(code->lines) <= 0)
    return;
  code_set_source(code->quoted_filename, code->linenum + 1);
  liter = list_iterate(code->lines);
  while(line = list_next(liter))
  {
    fprintf(output_handle, "%s", line);
    num_next_output_line++;
  }
  list_iterate_end(liter);
  code_reset_source();
}

static void
codeblock_out_brace(struct code_block *code)
{
  List_handle liter;                /* For list iteration. */
  char *line;                        /* Single code line. */

  if(list_len(code->lines) <= 0)
    return;
  code_set_source(code->quoted_filename, code->linenum);
  cout("{");
  liter = list_iterate(code->lines);
  while(line = list_next(liter))
  {
    fprintf(output_handle, "%s", line);
    num_next_output_line++;
  }
  list_iterate_end(liter);
  cout("}");
  code_reset_source();
}


struct code_block *
codeblock_new(void)
{
  struct code_block *cb;

  palloc(cb);
  cb->filename = NULL;
  cb->quoted_filename = NULL;
  cb->linenum  = -1;
  cb->lines    = list_create();
  return cb;
}

/*******************************************************************************
* Read a file and output it to the generated simulation code. Uses a
* fixed-size buffer, and will silently and arbitrarily break long lines.
*******************************************************************************/
static void
embed_file(char *name)
{
  char buf[4096];
  FILE *f;
  int last;

  if (!symtab_lookup(lib_instances, name))
  {
    /* First look in the system directory. */
    f = open_file_search_sys(name);
    /* If not found, look in the full search path. */
    if(f == NULL) {
      f = open_file_search(name);
      /* If still not found, abort. */
      if(f == NULL)
        fatal_error("Could not find file '%s'\n", name);
      else if (verbose) printf("Embedding file      %s (user path)\n", name);
    } else if (verbose) printf("Embedding file      %s (%s)\n", name, get_sys_dir());

    cout("");
    code_set_source(name, 1);
    /* Now loop, reading lines and outputting them in the code. */
    while(!feof(f))
    {
      if(fgets(buf, 4096, f) == NULL)
        break;
      last = strlen(buf) - 1;
      if(last >= 0 && (buf[last] == '\n' || buf[last] == '\r'))
        buf[last--] = '\0';
      if(last >= 0 && (buf[last] == '\n' || buf[last] == '\r'))
        buf[last--] = '\0';
      cout(buf);
    }
    fclose(f);
    coutf("/* End of file \"%s\". */", name);
    cout("");
    code_reset_source();
    symtab_add(lib_instances, name, NULL);
  } /* else file has already been embedded */
} /* embed_file */


/*******************************************************************************
* The following two functions output #define directives around a given piece
* of code to set up the right variable names (eg. the proper scope) for
* instrument and component parameters. The functions are recursive on the
* parameter lists.
*
* The functions first output an appropriate list of #define's, then call the
* supplied function func with the argument data, and finally outputs a
* matching list of #undef's.
*******************************************************************************/
static void cogen_instrument_scope_rec(List_handle parlist,
                                       void (*func)(void *), void *data)
{
  struct instr_formal *par;

  par = list_next(parlist);
  if(par != NULL)
  {
    if (strlen(par->id)) coutf("#define %s %sip%s", par->id, ID_PRE, par->id);
    cogen_instrument_scope_rec(parlist, func, data);
    if (strlen(par->id)) coutf("#undef %s", par->id);
  }
  else
  {
    (*func)(data);
  }
}

static void
cogen_instrument_scope(struct instr_def *instr,
                       void (*func)(void *), void *data)
{
  List_handle parlist;

  coutf("#define %scompcurname %s", ID_PRE, instr->name);
  /* This simply starts up the recursion on parameters. */
  parlist = list_iterate(instr->formals);
  cogen_instrument_scope_rec(parlist, func, data);
  list_iterate_end(parlist);
  coutf("#undef %scompcurname", ID_PRE);
}

/* Create the bindings for the SETTING parameter scope. Since the types of
* setting parameters are known, local declarations can be used, avoiding the
* problems with #define macro definitions.
* infunc=0: no definition of parameters
* infunc=1: define local copies of setting/definition parameters
* infunc=2: same as 1, but for TRACE adds the EXTEND block and handles JUMPs
*/
static void
cogen_comp_scope_setpar(struct comp_inst *comp, List_handle set, int infunc,
                        void (*func)(void *), void *data)
{
  struct comp_iformal *formal;

  /* Get the next setting parameter. */
  formal = list_next(set);
  if(formal != NULL)
  {
    /* Create local parameter equal to global value. */
    if(infunc)
      coutf("%s %s = %sc%s_%s;", instr_formal_type_names_real[formal->type], formal->id, ID_PRE, comp->name, formal->id);
    else
      coutf("#define %s %sc%s_%s", formal->id, ID_PRE, comp->name, formal->id);
    cogen_comp_scope_setpar(comp, set, infunc, func, data);

    if(!infunc)
      coutf("#undef %s", formal->id);
  }
  else
  {
    /* adds conditional execution for the TRACE */
    if (infunc==2 && comp->when) {
      coutf("/* '%s' component has conditional execution */",comp->name);
      coutf("if (%s)\n", exp_tostring(comp->when));
    }
    (*func)(data);                /* Now do the body. */
    if(infunc == 2 && list_len(comp->extend->lines) > 0)
    {
      coutf("/* '%s' component extend code */",comp->name);
      coutf("    SIG_MESSAGE(\"%s (Trace:Extend)\");", comp->name); /* signal handler message */
      if (comp->when)
        coutf("if (%s)\n", exp_tostring(comp->when));
      codeblock_out(comp->extend);
    }
  }
} /* cogen_comp_scope_setpar */

/* Create the #define statements to set up the scope for DEFINITION and OUTPUT
* parameters.
*/
static void
cogen_comp_scope_rec(struct comp_inst *comp, List_handle def, List set_list,
                     List_handle out, int infunc,
                     void (*func)(void *), void *data)
{
  char *par;
  struct comp_iformal *formal;

  /* First get the next DEFINITION or OUTPUT parameter, if any. */
  if(def != NULL)
  {
    formal = list_next(def);
    if(formal == NULL)
      def = NULL;                /* Now finished with definition parameters. */
    else
      par = formal->id;
  }
  if(def == NULL)
    par = list_next(out);
  if(par != NULL)
  {
    /* Create #define / #undef pair for this parameter around rest of code. */
    coutf("#define %s %sc%s_%s", par, ID_PRE, comp->name, par);
    cogen_comp_scope_rec(comp, def, set_list, out, infunc, func, data);
    coutf("#undef %s", par);
  }
  else
  { /* Now do the SETTING parameters. */
    List_handle set;

    if(infunc && list_len(set_list) > 0)
      coutf("{   /* Declarations of %s SETTING parameters. */", comp->name);
    set = list_iterate(set_list);
    cogen_comp_scope_setpar(comp, set, infunc, func, data);
    list_iterate_end(set);
    if(infunc && list_len(set_list) > 0)
      coutf("}   /* End of %s SETTING parameter declarations. */", comp->name);
  }
} /* cogen_comp_scope_rec */

static void
cogen_comp_scope(struct comp_inst *comp, int infunc,
                 void (*func)(void *), void *data)
{
  List_handle def, out;

  coutf("#define %scompcurname  %s", ID_PRE, comp->name);
  coutf("#define %scompcurtype  %s", ID_PRE, comp->type);
  coutf("#define %scompcurindex %i", ID_PRE, comp->index);
  def = list_iterate(comp->def->def_par);
  out = list_iterate(comp->def->out_par);
  cogen_comp_scope_rec(comp, def, comp->def->set_par, out,
                       infunc, func, data);
  list_iterate_end(out);
  list_iterate_end(def);
  coutf("#undef %scompcurname", ID_PRE);
  coutf("#undef %scompcurtype", ID_PRE);
  coutf("#undef %scompcurindex", ID_PRE);
} /* cogen_comp_scope */


/*******************************************************************************
* Generate declarations from users declaration section in component definition.
*******************************************************************************/
static void
cogen_comp_decls_doit(void *arg)
{
  struct comp_inst *comp = arg;

  /* Output the user declaration code block. */
  if (list_len(comp->def->decl_code->lines) > 0)
    codeblock_out(comp->def->decl_code);
}

static void
cogen_comp_decls(struct comp_inst *comp)
{
  cogen_comp_scope(comp, 0, cogen_comp_decls_doit, comp);
}

static void
cogen_comp_shares(struct comp_inst *comp)
{
  /* Output the 'share' declaration code block
    (once for all same components)*/

  if (comp->def->comp_inst_number < 0)
  {
    coutf("/* Shared user declarations for all components '%s'. */", comp->def->name);
    codeblock_out(comp->def->share_code);
    comp->def->comp_inst_number *= -1;
  }
}


/*******************************************************************************
* Generate declaration part of code.
*******************************************************************************/

static void
cogen_decls(struct instr_def *instr)
{
  List_handle liter;            /* For list iteration. */
  struct comp_iformal *c_formal;/* Name of component formal input parameter */
  struct instr_formal *i_formal;/* Name of instrument formal parameter. */
  struct comp_inst *comp;       /* Component instance. */
  int    index = 0;             /* index of comp instance */
  struct group_inst *group;     /* group instances */

  if (verbose) fprintf(stderr, "Writing instrument and components DECLARE\n");

  /* 1. Function prototypes. */
  coutf("void %sinit(void);", ID_PRE);
  coutf("neutron_t *%sraytrace(neutron_t *%sN);", ID_PRE, ID_PRE);
  coutf("void %ssave(FILE *);", ID_PRE);
  coutf("void %sfinally(void);", ID_PRE);
  coutf("void %sdisplay(void);", ID_PRE);
  cout("");

  /* 2. Component SHAREs. */
  liter = list_iterate(instr->complist);
  while(comp = list_next(liter))
  {
    if((list_len(comp->def->share_code->lines) > 0) && (comp->def->comp_inst_number < 0))
    {
      cogen_comp_shares(comp);
      cout("");
    }
  }
  list_iterate_end(liter);

  /* 3. Global variables for instrument parameters. */
  cout("/* Instrument parameters. */");
  liter = list_iterate(instr->formals);
  int numipar=0;
  while(i_formal = list_next(liter))
  {
    if (strlen(i_formal->id)) {
      coutf("%s " ID_PRE "ip%s;", instr_formal_type_names_real[i_formal->type], i_formal->id);
      numipar++;
    }
  }
  list_iterate_end(liter);
  cout("");

  /* 4. Table of instrument parameters. */
  coutf("#define %sNUMIPAR %d", ID_PRE, numipar);
  coutf("int %snumipar = %d;", ID_PRE, numipar);
  coutf("struct %sinputtable_struct %sinputtable[%sNUMIPAR+1] = {",
        ID_PRE, ID_PRE, ID_PRE);
  liter = list_iterate(instr->formals);
  while(i_formal = list_next(liter))
  {
    if (strlen(i_formal->id)) {
      if (i_formal->isoptional && !strcmp(instr_formal_type_names[i_formal->type],"instr_type_string"))
        coutf("  \"%s\", &%sip%s, %s, %s, ", i_formal->id, ID_PRE,
            i_formal->id, instr_formal_type_names[i_formal->type],
            exp_tostring(i_formal->default_value));
      else
        coutf("  \"%s\", &%sip%s, %s, \"%s\", ", i_formal->id, ID_PRE, i_formal->id,
            instr_formal_type_names[i_formal->type],
            i_formal->isoptional ? exp_tostring(i_formal->default_value) : "");
    }
  }
  list_iterate_end(liter);
  coutf("  NULL, NULL, instr_type_double, \"\"");
  coutf("};");  /* 5. Declaration of component definition and setting parameters. */
  cout("");

  /* 5. User's declarations from the instrument definition file. */
  cout("/* User declarations from instrument definition. */");
  cogen_instrument_scope(instr, (void (*)(void *))codeblock_out, instr->decls);
  cout("");

  /* 6. Table to store neutron states when entering each component */
  cout("/* Neutron state table at each component input (local coords) */");
  cout("/* [x, y, z, vx, vy, vz, t, sx, sy, sz, p] */");
  coutf("neutron_t* %scomp_storein;", ID_PRE);

  /* 7. Table to store position (abs/rel) for each component */
  cout("/* Components position table (absolute and relative coords) */");
  coutf("Coords %scomp_posa[%i];", ID_PRE, list_len(instr->complist)+2);
  coutf("Coords %scomp_posr[%i];", ID_PRE, list_len(instr->complist)+2);
  cout("/* Counter for each comp to check for inactive ones */");
  coutf("MCNUM  %sNCounter[%i];", ID_PRE, list_len(instr->complist)+2);
  coutf("MCNUM  %sPCounter[%i];", ID_PRE, list_len(instr->complist)+2);
  coutf("MCNUM  %sP2Counter[%i];", ID_PRE, list_len(instr->complist)+2);
  coutf("#define %sNUMCOMP %d /* number of components */", ID_PRE, list_len(instr->complist)+1);
  cout("/* Counter for PROP ABSORB */");
  coutf("MCNUM  %sAbsorbProp[%i];", ID_PRE, list_len(instr->complist)+2);

  if (list_len(instr->grouplist) > 0)
  {
    cout("/* Component group definitions (flags), equals index of scattering comp */");
    liter = list_iterate(instr->grouplist);
    while(group = list_next(liter))
    {
      coutf("int %sGroup%s=0;", ID_PRE, group->name);
    }
    list_iterate_end(liter);
  }

  /* 9. Declaration of component definition/setting parameters */
  cout("/* Declarations of component definition and setting parameters. */");
  cout("");
  index = 0;


  liter = list_iterate(instr->complist);
  while(comp = list_next(liter))
  {
    List_handle liter2;

    index++;
    comp->index = index; /* should match the one defined with bison */

    if(list_len(comp->def->def_par) > 0)
    {                                /* (The if avoids a redundant comment.) */
      coutf("/* Definition parameters for component '%s' [%i]. */", comp->name, comp->index);
      liter2 = list_iterate(comp->def->def_par);
      while(c_formal = list_next(liter2))
      {
        struct Symtab_entry *entry = symtab_lookup(comp->defpar, c_formal->id);
        char *val = exp_tostring(entry->val);
        if (c_formal->type != instr_type_string)
        	coutf("#define %sc%s_%s %s", ID_PRE, comp->name, c_formal->id, val);
        else {
          /* a string definition parameter is concerted into a setting parameter to avoid e.g.
           * warning: format ‘%s’ expects type ‘char *’, but argument X has type ‘int’    */
          fprintf(stderr,"Warning: Component %s definition parameter (string) %s\n"
                         "         may be changed into a setting parameter to avoid warnings at compile time.\n",
          	comp->name, c_formal->id);
        	coutf("#define %sc%s_%s %s /* declared as a string. May produce warnings at compile */", ID_PRE, comp->name, c_formal->id, val);
				}
        str_free(val);
      }
      list_iterate_end(liter2);
    }
    if(list_len(comp->def->set_par) > 0)
    {
      coutf("/* Setting parameters for component '%s' [%i]. */", comp->name, comp->index);
      liter2 = list_iterate(comp->def->set_par);
      while(c_formal = list_next(liter2))
      {
        if (c_formal->type != instr_type_string)
          coutf("%s %sc%s_%s;", instr_formal_type_names_real[c_formal->type], ID_PRE, comp->name, c_formal->id);
        else  /* char type for component */
          coutf("%s %sc%s_%s[16384];", instr_formal_type_names_real[c_formal->type+1], ID_PRE, comp->name, c_formal->id);
      }
      list_iterate_end(liter2);
    }
    if(list_len(comp->def->def_par) > 0 || list_len(comp->def->set_par) > 0)
      cout("");
  }
  list_iterate_end(liter);

  /* 10. User declarations from component definitions (for each instance). */
  cout("/* User component declarations. */");
  cout("");
  liter = list_iterate(instr->complist);
  while(comp = list_next(liter))
  {
    /* set target names for jumps and define iteration counters */
    if(list_len(comp->jump) > 0) {
      struct jump_struct *this_jump;
      int    jump_index=0;
      List_handle liter2;
      liter2 = list_iterate(comp->jump);
      while(this_jump = list_next(liter2)) {
        List_handle liter3;
        char *jump_target_type=NULL;
        jump_index++;
        struct comp_inst *comp3;
        /* check name/type of target */
        liter3 = list_iterate(instr->complist);
        while(comp3 = list_next(liter3)) {
          if ((this_jump->target && !strcmp(this_jump->target, comp3->name))
           || (!this_jump->target && comp3->index == comp->index+this_jump->target_index) ) {
            if (!this_jump->target) this_jump->target=comp3->name;
            jump_target_type =comp3->type;
            break;
          }
        }
        list_iterate_end(liter3);

        this_jump->index = jump_index;
        if (!this_jump->target || !jump_target_type) {
          fatal_error("JUMP %i (relative %i) from component %s "
                      "is not in the instrument.\n",
            this_jump->index, this_jump->target_index, comp->name);
        }
        /* JUMP are valid only if MYSELF or between Arm's */
        if (!(!strcmp(this_jump->target, comp->name) ||
        (!strcmp(jump_target_type, "Arm")) ))
          fatal_error("JUMPs can only apply on MYSELF or to Arm components.\n"
                      "  Target %s is a %s (not an Arm).\n",
            this_jump->target, jump_target_type);
        /* create counter for iteration */
        if (this_jump->iterate)
          coutf("long %sJumpCounter%s_%i;",
            ID_PRE, comp->name, this_jump->index);
        fprintf(stderr,"Info:    Defining %s JUMP from %s to %s\n",
          (this_jump->iterate ? "iterative" : "conditional"),
          comp->name, this_jump->target);
      }
      list_iterate_end(liter2);
    }

    if((list_len(comp->def->decl_code->lines) > 0) || (comp->def->comp_inst_number < 0))
    {
      coutf("/* User declarations for component '%s' [%i]. */", comp->name, comp->index);
      cogen_comp_decls(comp);
      cout("");
    }
  }
  list_iterate_end(liter);

  /* 11. Declarations for the position and rotation transformations between
     coordinate systems of components. */
  liter = list_iterate(instr->complist);
  while(comp = list_next(liter))
  {
    coutf("Coords %sposa%s, %sposr%s;", ID_PRE, comp->name, ID_PRE, comp->name);
    coutf("Rotation %srota%s, %srotr%s;", ID_PRE, comp->name, ID_PRE, comp->name);
  }
  list_iterate_end(liter);
  cout("");

  cout("/* end declare */");
  cout("");

} /* cogen_decls */

/*******************************************************************************
* cogen_init: Generate the INIT section.
*******************************************************************************/
static void
cogen_init(struct instr_def *instr)
{
  List_handle liter;
  struct comp_inst *comp, *last;
  char *d2r;

  if (verbose) fprintf(stderr, "Writing instrument and components INITIALIZE\n");

  coutf("void %sinit(void) {", ID_PRE);

  /* User initializations from instrument definition. */
  cogen_instrument_scope(instr, (void (*)(void *))codeblock_out_brace,
                         instr->inits);

  /* MOD: E. Farhi Sep 20th, 2001 moved transformation block so that */
  /*                              it can be used in following init block */

  /* Initialize neutron storage arrays */
  coutf(  "%scomp_storein = malloc(sizeof(neutron_t) * (%i + 2));",
		  ID_PRE, list_len(instr->complist));

  /* Compute the necessary vectors and transformation matrices for coordinate
     system changes between components. */
  cout("  /* Computation of coordinate transformations. */");
  cout("  {");
  coutf("    Coords %stc1, %stc2;", ID_PRE, ID_PRE);
  coutf("    Rotation %str1;", ID_PRE);
  cout("");
  /* Conversion factor degrees->radians for rotation angles. */
  d2r = "DEG2RAD";

  liter = list_iterate(instr->complist);
  last = NULL;
  coutf("    %sDEBUG_INSTR()", ID_PRE);

  while((comp = list_next(liter)) != NULL)
  {
    struct comp_inst *relcomp; /* Component relative to. */
    char *x, *y, *z;

    coutf("    /* Component %s. */", comp->name);
    coutf("    SIG_MESSAGE(\"%s (Init:Place/Rotate)\");", comp->name); /* signal handler message */

    /* Absolute rotation. */
    x = exp_tostring(comp->pos->orientation.x);
    y = exp_tostring(comp->pos->orientation.y);
    z = exp_tostring(comp->pos->orientation.z);
    relcomp = comp->pos->orientation_rel;
    if(relcomp == NULL)
    {                                /* Absolute orientation. */
      coutf("    rot_set_rotation(%srota%s,", ID_PRE, comp->name);
      code_set_source(instr->quoted_source,
                      exp_getlineno(comp->pos->orientation.x));
      coutf("      (%s)*%s,", x, d2r);
      code_set_source(instr->quoted_source,
                      exp_getlineno(comp->pos->orientation.y));
      coutf("      (%s)*%s,", y, d2r);
      code_set_source(instr->quoted_source,
                      exp_getlineno(comp->pos->orientation.z));
      coutf("      (%s)*%s);", z, d2r);
      code_reset_source();
    }
    else
    {
      coutf("    rot_set_rotation(%str1,", ID_PRE);
      code_set_source(instr->quoted_source,
                      exp_getlineno(comp->pos->orientation.x));
      coutf("      (%s)*%s,", x, d2r);
      code_set_source(instr->quoted_source,
                      exp_getlineno(comp->pos->orientation.y));
      coutf("      (%s)*%s,", y, d2r);
      code_set_source(instr->quoted_source,
                      exp_getlineno(comp->pos->orientation.z));
      coutf("      (%s)*%s);", z, d2r);
      code_reset_source();
      coutf("    rot_mul(%str1, %srota%s, %srota%s);",
            ID_PRE, ID_PRE, relcomp->name, ID_PRE, comp->name);
    }
    str_free(z);
    str_free(y);
    str_free(x);

    /* Relative rotation. */
    if(last == NULL)
    {                                /* First component. */
      coutf("    rot_copy(%srotr%s, %srota%s);",
            ID_PRE, comp->name, ID_PRE, comp->name);
    }
    else
    {
      coutf("    rot_transpose(%srota%s, %str1);", ID_PRE, last->name, ID_PRE);
      coutf("    rot_mul(%srota%s, %str1, %srotr%s);",
            ID_PRE, comp->name, ID_PRE, ID_PRE, comp->name);
    }

    /* Absolute position. */
    x = exp_tostring(comp->pos->place.x);
    y = exp_tostring(comp->pos->place.y);
    z = exp_tostring(comp->pos->place.z);
    relcomp = comp->pos->place_rel;
    if(relcomp == NULL)
    {
      coutf("    %sposa%s = coords_set(", ID_PRE, comp->name);
      code_set_source(instr->quoted_source, exp_getlineno(comp->pos->place.x));
      coutf("      %s,", x);
      code_set_source(instr->quoted_source, exp_getlineno(comp->pos->place.y));
      coutf("      %s,", y);
      code_set_source(instr->quoted_source, exp_getlineno(comp->pos->place.z));
      coutf("      %s);", z);
      code_reset_source();
    }
    else
    {
      coutf("    %stc1 = coords_set(", ID_PRE);
      code_set_source(instr->quoted_source, exp_getlineno(comp->pos->place.x));
      coutf("      %s,", x);
      code_set_source(instr->quoted_source, exp_getlineno(comp->pos->place.y));
      coutf("      %s,", y);
      code_set_source(instr->quoted_source, exp_getlineno(comp->pos->place.z));
      coutf("      %s);", z);
      code_reset_source();
      coutf("    rot_transpose(%srota%s, %str1);",
            ID_PRE, relcomp->name, ID_PRE);
      coutf("    %stc2 = rot_apply(%str1, %stc1);",
            ID_PRE, ID_PRE, ID_PRE);
      coutf("    %sposa%s = coords_add(%sposa%s, %stc2);",
            ID_PRE, comp->name, ID_PRE, relcomp->name, ID_PRE);
    }

    str_free(z);
    str_free(y);
    str_free(x);

    /* Relative position. */
    if(last == NULL)
      coutf("    %stc1 = coords_neg(%sposa%s);", ID_PRE, ID_PRE, comp->name);
    else
      coutf("    %stc1 = coords_sub(%sposa%s, %sposa%s);",
            ID_PRE, ID_PRE, last->name, ID_PRE, comp->name);
    coutf("    %sposr%s = rot_apply(%srota%s, %stc1);",
          ID_PRE, comp->name, ID_PRE, comp->name, ID_PRE);

    coutf("    %sDEBUG_COMPONENT(\"%s\", %sposa%s, %srota%s)",
          ID_PRE, comp->name, ID_PRE, comp->name, ID_PRE, comp->name);

    coutf("    %scomp_posa[%i] = %sposa%s;", ID_PRE, comp->index, ID_PRE, comp->name);
    coutf("    %scomp_posr[%i] = %sposr%s;", ID_PRE, comp->index, ID_PRE, comp->name);
    coutf("    %sNCounter[%i]  = %sPCounter[%i] = %sP2Counter[%i] = 0;",
          ID_PRE, comp->index, ID_PRE, comp->index, ID_PRE, comp->index);
    coutf("    %sAbsorbProp[%i]= 0;", ID_PRE, comp->index);

    last = comp;
  }
  list_iterate_end(liter);

  /* Initialization of component setting parameters and user initialization
     code. */
  cout("  /* Component initializations. */");
  liter = list_iterate(instr->complist);
  while((comp = list_next(liter)) != NULL)
  {
    List_handle setpar;
    struct comp_iformal *par;

    coutf("  /* Initializations for component %s. */", comp->name);
    coutf("  SIG_MESSAGE(\"%s (Init)\");", comp->name); /* signal handler message */
    /* Initialization of the component setting parameters. */
    setpar = list_iterate(comp->def->set_par);
    while((par = list_next(setpar)) != NULL)
    {
      char *val;
      struct Symtab_entry *entry;

      entry = symtab_lookup(comp->setpar, par->id);
      val = exp_tostring(entry->val);
      code_set_source(instr->quoted_source, exp_getlineno(entry->val));
      if (par->type != instr_type_string)
      {
        coutf("  %sc%s_%s = %s;", ID_PRE, comp->name, par->id, val);
      }
      else
      {
        coutf("  if(%s) strncpy(%sc%s_%s,%s, 16384); else %sc%s_%s[0]='\\0';", val, ID_PRE, comp->name, par->id, val, ID_PRE, comp->name, par->id);
      }
      str_free(val);
    }
    list_iterate_end(setpar);
    if(list_len(comp->def->set_par) > 0)
      code_reset_source();
    cout("");

    /* Users initializations. */
    if(list_len(comp->def->init_code->lines) > 0)
      cogen_comp_scope(comp, 0, (void (*)(void *))codeblock_out_brace,
                            comp->def->init_code);
    cout("");
  }
  list_iterate_end(liter);

  /* Output graphics representation of components. */
  coutf("    if(%sdotrace) %sdisplay();", ID_PRE, ID_PRE);
  coutf("    %sDEBUG_INSTR_END()", ID_PRE);
  cout("  }");
  cout("");
  cout ("/* NeXus support */\n");
  cout ("#ifdef USE_NEXUS\n");
  coutf("strncmp(%snxversion,\"%s\",128);\n", ID_PRE,
    instr->nxinfo->any && instr->nxinfo->hdfversion && strlen(instr->nxinfo->hdfversion) ?
      instr->nxinfo->hdfversion : "5 zip");
  cout ("#endif\n");

  cout("} /* end init */");
  cout("");
} /* cogen_init */

/** 
 * Create a function for each part of each component.
 */
/* {{{ cogen_comp_funct */
static void cogen_comp_funct(struct instr_def *instr) {
	struct comp_inst *comp;
	int i;

	/* Handle state parameter names. */
	char *statepars[10];
	static char *statepars_names[10] =
	{
		"pos.x", "pos.y", "pos.z", "vel.x", "vel.y", "vel.z",
		"t", "pol.x", "pol.y", "p"
	};
	List_handle statepars_handle;

	List_handle liter = list_iterate(instr->complist);
	while((comp = list_next(liter)) != NULL) {

		/* Write out a function for the component */
		coutf("/* Functions for '%s' ('%s') */",
				comp->name, comp->type);
		coutf("uint component_%d_%s(neutron_t *%sN) {", comp->index, comp->name, ID_PRE);

		coutf("#define mcabsorb mcabsorb_comp%d", comp->index);

		/* {{{ Unfold mcN */
		cout("  /* Unfold neutron struct to 'free' variables. */");

		/* Set up the user-defined variables. */
		for(i = 0; i < 10; i++) statepars[i] = NULL;
		statepars_handle = list_iterate(comp->def->state_par);
		for(i = 0; i < 10; i++) {
			statepars[i] = list_next(statepars_handle);
			if(statepars[i] == NULL)
				break;
		}
		list_iterate_end(statepars_handle);

		/* If more than one parameter, unpack the struct. */
		if(list_len(comp->def->state_par) != 1) {
			for(i = 0; i < 10; i++)
			{
				if(statepars[i] != NULL)
					coutf("  MCNUM %s = %sN->%s;", statepars[i], ID_PRE, statepars_names[i]);
				else
					break;
			}

			coutf("  #define %snlx x", ID_PRE);
			coutf("  #define %snly y", ID_PRE);
			coutf("  #define %snlz z", ID_PRE);
			coutf("  #define %snlvx vx", ID_PRE);
			coutf("  #define %snlvy vy", ID_PRE);
			coutf("  #define %snlvz vz", ID_PRE);
			coutf("  #define %snlt t", ID_PRE);
			coutf("  #define %snlsx sx", ID_PRE);
			coutf("  #define %snlsy sy", ID_PRE);
			coutf("  #define %snlsz sz", ID_PRE);
			coutf("  #define %snlp p", ID_PRE);

			coutf("  #define SCATTERED %sN->scatter", ID_PRE);
			coutf("  #define %sScattered %sN->scatter", ID_PRE, ID_PRE);

			/* Polarization ? */
			if(comp->def->polarisation_par)
			{
				coutf("  MCNUM %s = %sN->pol.x;", comp->def->polarisation_par[0], ID_PRE);
				coutf("  MCNUM %s = %sN->pol.y;", comp->def->polarisation_par[1], ID_PRE);
				coutf("  MCNUM %s = %sN->pol.z;", comp->def->polarisation_par[2], ID_PRE);
			} else {
				coutf("  MCNUM sx = %sN->pol.x;", ID_PRE);
				coutf("  MCNUM sy = %sN->pol.y;", ID_PRE);
				coutf("  MCNUM sz = %sN->pol.z;", ID_PRE);
			}
			/* Only one parameter -- use the struct directly. */
		} else {
			coutf("  neutron_t* %s = %sN;", statepars[0], ID_PRE);

			coutf("  #define %snlx %sN->pos.x", ID_PRE, ID_PRE);
			coutf("  #define %snly %sN->pos.y", ID_PRE, ID_PRE);
			coutf("  #define %snlz %sN->pos.z", ID_PRE, ID_PRE);
			coutf("  #define %snlvx %sN->vel.x", ID_PRE, ID_PRE);
			coutf("  #define %snlvy %sN->vel.y", ID_PRE, ID_PRE);
			coutf("  #define %snlvz %sN->vel.z", ID_PRE, ID_PRE);
			coutf("  #define %snlt %sN->t", ID_PRE, ID_PRE);
			coutf("  #define %snlsx %sN->pol.x", ID_PRE, ID_PRE);
			coutf("  #define %snlsy %sN->pol.y", ID_PRE, ID_PRE);
			coutf("  #define %snlsz %sN->pol.z", ID_PRE, ID_PRE);
			coutf("  #define %snlp %sN->p", ID_PRE, ID_PRE);

			coutf("  #define SCATTERED %sN->scatter", ID_PRE);
			coutf("  #define %sScattered %sN->scatter", ID_PRE, ID_PRE);


		}
		cout("");
		/* }}} Unfold mcN */

		/* write component parameters and trace+extend code */
		/* also handles jumps after extend */
		cout("/* {{{ cogen_comp_scope - codeblock_out_brace */");
		cogen_comp_scope(comp, 2, (void (*)(void *))codeblock_out_brace,
				comp->def->trace_code);
		cout("/* }}} cogen_comp_scope - codeblock_out_brace */");

		/* {{{ Refold mcN */
		coutf("  #undef %snlx", ID_PRE);
		coutf("  #undef %snly", ID_PRE);
		coutf("  #undef %snlz", ID_PRE);
		coutf("  #undef %snlvx", ID_PRE);
		coutf("  #undef %snlvy", ID_PRE);
		coutf("  #undef %snlvz", ID_PRE);
		coutf("  #undef %snlp", ID_PRE);
		coutf("  #undef %snlsx", ID_PRE);
		coutf("  #undef %snlsy", ID_PRE);
		coutf("  #undef %snlsz", ID_PRE);
		coutf("  #undef %snlt", ID_PRE);

		coutf("  #undef SCATTERED");
		coutf("  #undef %sScattered", ID_PRE);

		if(list_len(comp->def->state_par) != 1) {
			cout("  /* Re-fold variables to neutron struct */");
						for(i = 0; i < 10; i++)
			{
				if(statepars[i] != NULL)
					coutf("  %sN->%s = %s;", ID_PRE, statepars_names[i], statepars[i]);
				else
					break;
			}

			/* Polarization ? */
			if(comp->def->polarisation_par)
			{
				coutf("  %sN->pol.x = %s;", ID_PRE, comp->def->polarisation_par[0]);
				coutf("  %sN->pol.y = %s;", ID_PRE, comp->def->polarisation_par[1]);
				coutf("  %sN->pol.z = %s;", ID_PRE, comp->def->polarisation_par[2]);
			} else {
				coutf("  %sN->pol.x = sx;", ID_PRE);
				coutf("  %sN->pol.y = sy;", ID_PRE);
				coutf("  %sN->pol.z = sz;", ID_PRE);
			}
		}
		cout("");
		/* }}} Refold mcN */

		cout("#undef mcabsorb");

		/* Debugging (exit from component). */
		coutf("  %sDEBUG_STATE_nt(%sN)", ID_PRE, ID_PRE);

		cout("  return 0;");

		coutf("mcabsorb_comp%d:", comp->index);
		coutf("%sDEBUG_STATE_nt(%sN)", ID_PRE, ID_PRE);
		cout("return 1;");

		coutf("}");
		cout("");
	}
	list_iterate_end(liter);

} /* }}} cogen_comp_funct */

/*******************************************************************************
* cogen_trace: Generate the TRACE section.
* Extended Grammar: uses goto and labels and installs tests
*   WHEN: the trace section of comp is embraced in a: if (when) { ...  }
*   GROUP: defines a global Group_<name> flag which gets true when one of the
*          comps SCATTER. Rest of GROUP is then skipped, using goto's.
*          ABSORB neutrons are sent to label absorbComp at the end of component
*          and next comp in GROUP is tested.
*   JUMP:  sends neutron to the JumpTrace labels, either with condition
*          or condition is (counter < iterations)
*   SPLIT: loops from comp/group TRACE to END, incrementing mcrun_num
*******************************************************************************/
static void
cogen_trace(struct instr_def *instr)
{
  List_handle liter;
  struct comp_inst *comp;
  struct group_inst *group;

  /* Variables for inserting JUMP-statements */
  List_handle liter_jumps;
  struct jump_struct *this_jump;

  if (verbose) fprintf(stderr, "Writing instrument and components TRACE\n");

  /* Output the function header. */
  coutf("neutron_t *%sraytrace(neutron_t *%sN) {", ID_PRE, ID_PRE);
  /* Local neutron state. */
  cout("  uint result = 0;");

  /* Debugging (initial state). */
  coutf("  %sDEBUG_ENTER()", ID_PRE);
  coutf("  %sDEBUG_STATE_nt(%sN)", ID_PRE, ID_PRE);

  /* Set group flags */
  if (list_len(instr->grouplist) > 0)
  {
    cout("/* Set Component group definitions (flags) */");
    liter = list_iterate(instr->grouplist);
    while(group = list_next(liter))
    {
      coutf("  %sGroup%s=0; /* equals index of scattering comp when in group */", ID_PRE, group->name);
    }
    list_iterate_end(liter);
  }

  /* default is the normal ABSORB to end of TRACE */
  coutf("#define %sabsorb %sabsorbAll", ID_PRE, ID_PRE);

  /* initiate iteration counters for each TRACE */
  liter = list_iterate(instr->complist);
  while((comp = list_next(liter)) != NULL)
  {
    if(list_len(comp->jump) > 0) {
      struct jump_struct *this_jump;
      List_handle liter2;
      liter2 = list_iterate(comp->jump);
      while(this_jump = list_next(liter2)) {
        /* create counter for iteration */
        if (this_jump->iterate)
          coutf("  %sJumpCounter%s_%i=0;",
            ID_PRE, comp->name, this_jump->index);
      }
      list_iterate_end(liter2);
    }
    /* if comp is in a split GROUP, install counter only for first comp of GROUP */
    if (comp->group && comp->group->split) {
      if (!strcmp(comp->name, comp->group->first_comp))
        comp->split      = comp->group->split;
      else comp->split   = NULL;
    }
    if (comp->split) {
      coutf("  /* SPLIT counter for component %s */", comp->name);
      coutf("  int %sSplit_%s=0;", ID_PRE, comp->name);
      fprintf(stderr,"Info:    Defining SPLIT from %s to END in instrument %s\n",
          comp->name, instr->name);
    }
  }
  list_iterate_end(liter);

  /* {{{ Trace components */
  liter = list_iterate(instr->complist);
  while((comp = list_next(liter)) != NULL)
  {
    coutf("  /* TRACE Component %s [%i] */", comp->name, comp->index);

	/* Change of coordinates. */
	coutf("  %scoordschange_nt(%sposr%s, %srotr%s, %sN);", ID_PRE, ID_PRE, comp->name, ID_PRE, comp->name, ID_PRE);

    if(instr->polarised) {
	  coutf("  %scoordschange_polarisation_nt("
			  "%srotr%s, %sN);",
        ID_PRE, ID_PRE, comp->name, ID_PRE);
    }

    /* JUMP RELATIVE comp */
    coutf("  /* define label inside component %s (without coords transformations) */", comp->name);
    coutf("  %sJumpTrace_%s:", ID_PRE, comp->name);

	/* Add iterative jumps */
	liter_jumps = list_iterate(comp->jump);
	while(this_jump = list_next(liter_jumps)) {
		char *exp = exp_tostring(this_jump->condition);
		if (this_jump->iterate) {
			cout("  /* Processing iterate-jumps - is inserted slightly different than normal... */");
			coutf("  for(%sJumpCounter%s_%i=0; %sJumpCounter%s_%i < %s; %sJumpCounter%s_%i++) {",
					ID_PRE, comp->name, this_jump->index,
					ID_PRE, comp->name, this_jump->index, exp,
					ID_PRE, comp->name, this_jump->index);
		}
	}
	list_iterate_end(liter_jumps);

    coutf("  SIG_MESSAGE(\"%s (Trace)\");", comp->name); /* signal handler message */
    coutf("  %sDEBUG_COMP(\"%s\")", ID_PRE, comp->name);

    /* Debugging (entry into component). */
    coutf("  %sDEBUG_STATE_nt(%sN)", ID_PRE, ID_PRE);

	/* store neutron state in mccomp_storein */
	cout("/* {{{ Processing splits */");
    if (!comp->split) {
	coutf("  %sstore_neutron_nt(%scomp_storein, %i, %sN);",
			ID_PRE, ID_PRE, comp->index, ID_PRE);
	} else {
      /* spliting: store first time, then restore neutron */
      char *exp=exp_tostring(comp->split); /* number of splits */
      coutf("  if (!%sSplit_%s) {                   /* STORE only the first time */", ID_PRE, comp->name);
      coutf("    if (floor(%s) > 1) %sN->p /= floor(%s); /* adapt weight for SPLITed neutron */", exp, ID_PRE, exp);
	  coutf("  %sstore_neutron_nt(%scomp_storein, %i, %sN);",
			  ID_PRE, ID_PRE, comp->index, ID_PRE);
      coutf("  } else {");
	  coutf("  %srestore_neutron_nt(%scomp_storein, %i, %sN);",
			ID_PRE, ID_PRE, comp->index, ID_PRE);
	  cout("  }");
      coutf("  %sSplit_%s++; /* SPLIT number */", ID_PRE, comp->name);
      str_free(exp);
    }
	cout("/* }}} Processing splits */");

    coutf("  %sN->scatter = 0;", ID_PRE);
    coutf("  %sNCounter[%i]++;", ID_PRE, comp->index);
    coutf("  %sPCounter[%i] += %sN->p;", ID_PRE, comp->index, ID_PRE);
    coutf("  %sP2Counter[%i] += (%sN->p)*(%sN->p);", ID_PRE, comp->index, ID_PRE, ID_PRE);

    if (comp->group)
    {
      coutf("  if (!%sGroup%s) { /* previous comps of GROUP have not SCATTERED yet */", ID_PRE, comp->group->name);
      coutf("#undef %sabsorb", ID_PRE);
      cout ("/* if ABSORBed in GROUP/comp, will go to end of component */");
      coutf("#define %sabsorb %sabsorbComp%s", ID_PRE, ID_PRE, comp->name);

    }

    /* Call the component */
    coutf("  result = component_%d_%s(%sN);", comp->index, comp->name, ID_PRE);
    coutf("  if(result == 1) { goto mcabsorbAll; }");

    if (comp->group) {
      coutf("#undef %sabsorb", ID_PRE);
      coutf("#define %sabsorb %sabsorbAll", ID_PRE, ID_PRE);
      coutf("  } /* end comp %s in GROUP %s */", comp->name, comp->group->name);
      coutf("  if (%sN->scatter) %sGroup%s=%i;",
        ID_PRE, ID_PRE, comp->group->name, comp->index);
      cout ("  /* Label to skip component instead of ABSORB */");
      coutf("  %sabsorbComp%s:", ID_PRE, comp->name);
      
      if (strcmp(comp->name, comp->group->last_comp)) {
        /* not the last comp of GROUP: check if SCATTERED */
        coutf("  if (!%sGroup%s) /* restore neutron if was not scattered in GROUP yet */", ID_PRE, comp->group->name);
		coutf("    { %srestore_neutron_nt(%scomp_storein, %i, %sN); }",
			ID_PRE, ID_PRE, comp->index, ID_PRE);
      } else {
        /* last comp of GROUP: restore default ABSORB */
        coutf("/* end of GROUP %s */", comp->group->name);
        coutf("  if (!%sGroup%s) ABSORB; /* absorb neutrons non scattered in GROUP */", ID_PRE, comp->group->name);
      }
    }

	/* Conditional jumps and end of iterative jumps */
	liter_jumps = list_iterate(comp->jump);
	while(this_jump = list_next(liter_jumps)) {
		char *exp = exp_tostring(this_jump->condition);
		if (this_jump->iterate) {
			coutf("  } /* %sJumpCounter%s_%i */",
					ID_PRE, comp->name, this_jump->index);
		} else {
			/* A conditional jump */
			cout("  {");
			coutf("    neutron_t* N = %sN;", ID_PRE);
			coutf("    if (%s) goto %sJumpTrace_%s;",
					exp, ID_PRE, this_jump->target);
			cout("  }");
		}
	}
	list_iterate_end(liter_jumps);

    /* Debugging (exit from component). */
    coutf("  %sDEBUG_STATE_nt(%sN)", ID_PRE, ID_PRE);
    cout("");
  }
  list_iterate_end(liter);
  /* }}} Trace components */

  /* SPLITing: should loop components if required */
  liter = list_iterate(instr->complist);
  char *reverse_SplitJumps = str_dup("");
  char has_splits=0;
  while((comp = list_next(liter)) != NULL)
  {
    if (comp->split) {
      has_splits = 1;
      char *exp=exp_tostring(comp->split); /* number of splits */
      char line[256];
      char cat_line[1024]; strcpy(cat_line, "");
      sprintf(line,"  if (%sSplit_%s && %sSplit_%s < (%s)) {\n",
        ID_PRE, comp->name, ID_PRE, comp->name, exp);
      strcat(cat_line, line);
      if (comp->group) {
        sprintf(line,"    %sGroup%s=0;\n", ID_PRE, comp->group->name);
        strcat(cat_line, line);
      }
      sprintf(line,"    goto %sJumpTrace_%s;\n  }\n", ID_PRE, comp->name);
      strcat(cat_line, line);
      sprintf(line,"    else %sSplit_%s=0;\n", ID_PRE, comp->name);
      strcat(cat_line, line);
      char *tmp=str_cat(cat_line, reverse_SplitJumps, NULL);
      str_free(reverse_SplitJumps); reverse_SplitJumps = tmp;
      str_free(exp);
    }
  }
  list_iterate_end(liter);

  /* Absorbing neutrons - goto this label to skip remaining components. End of TRACE */
  coutf("  %sabsorbAll:", ID_PRE);

  if (has_splits) coutf("  /* SPLIT loops in reverse order */\n%s", reverse_SplitJumps);

  /* Debugging (final state). */
  coutf("  %sDEBUG_LEAVE()", ID_PRE);
  coutf("  %sDEBUG_STATE_nt(%sN)", ID_PRE, ID_PRE);

  /* ToDo: Currently, this will be in the local coordinate system of the last
     component - should be transformed back into the global system. */
  coutf("  return %sN;", ID_PRE);

  /* Function end. */
  cout("} /* end trace */");
  cout("");
} /* cogen_trace */
/*******************************************************************************
* cogen_save: Generate the SAVE section.
*******************************************************************************/
static void
cogen_save(struct instr_def *instr)
{
  List_handle liter;             /* For list iteration. */
  struct comp_inst *comp;        /* Component instance. */

  if (verbose) fprintf(stderr, "Writing instrument and components SAVE\n");

  /* User SAVE code from component definitions (for each instance). */
  coutf("void %ssave(FILE *handle) {", ID_PRE);
  /* In case the save occurs during simulation (-USR2 not at end), we must close
   * current siminfo and re-open it, not to have redundant monitor entries
   * saved each time for each monitor. The sim_info is then incomplete, but
   * data is saved entirely. It is completed during the last siminfo_close
   * of mcraytrace
   */
  coutf("  if (!handle) %ssiminfo_init(NULL);", ID_PRE);
  cout("  /* User component SAVE code. */");
  cout("");
  liter = list_iterate(instr->complist);
  while(comp = list_next(liter))
  {
    if(list_len(comp->def->save_code->lines) > 0)
    {
      coutf("  /* User SAVE code for component '%s'. */", comp->name);
      coutf("  SIG_MESSAGE(\"%s (Save)\");", comp->name); /* signal handler message */
      cogen_comp_scope(comp, 1, (void (*)(void *))codeblock_out_brace,
                       comp->def->save_code);
      cout("");
    }
  }
  list_iterate_end(liter);

  /* User's SAVE code from the instrument definition file. */
  if(list_len(instr->saves->lines) > 0)
  {
    cout("  /* User SAVE code from instrument definition. */");
    coutf("  SIG_MESSAGE(\"%s (Save)\");", instr->name);
    cogen_instrument_scope(instr, (void (*)(void *))codeblock_out_brace,
                           instr->saves);
    cout("");
  }
  coutf("  if (!handle) %ssiminfo_close(); ", ID_PRE);
  cout("} /* end save */");
} /* cogen_save */

/*******************************************************************************
* cogen_finally: Generate the FINALLY section.
*******************************************************************************/
static void
cogen_finally(struct instr_def *instr)
{
  List_handle liter;                /* For list iteration. */
  struct comp_inst *comp;           /* Component instance. */

  if (verbose) fprintf(stderr, "Writing instrument and components FINALLY\n");

  /* User FINALLY code from component definitions (for each instance). */
  coutf("void %sfinally(void) {", ID_PRE);
  cout("  /* De-allocate used structures. */");
  coutf("  free(%scomp_storein);", ID_PRE);
  cout("");

  cout("  /* User component FINALLY code. */");
  /* first call SAVE code to save any remaining data */
  coutf("  %ssiminfo_init(NULL);", ID_PRE);
  coutf("  %ssave(%ssiminfo_file); /* save data when simulation ends */", ID_PRE, ID_PRE);
  cout("");
  liter = list_iterate(instr->complist);
  while(comp = list_next(liter))
  {
    if(list_len(comp->def->finally_code->lines) > 0)
    {
      coutf("  /* User FINALLY code for component '%s'. */", comp->name);
      coutf("  SIG_MESSAGE(\"%s (Finally)\");", comp->name); /* signal handler message */
      cogen_comp_scope(comp, 1, (void (*)(void *))codeblock_out_brace,
                       comp->def->finally_code);
      cout("");
    }
    coutf("    if (!%sNCounter[%i]) "
      "fprintf(stderr, \"Warning: No neutron could reach Component[%i] %s\\n\");",
      ID_PRE, comp->index, comp->index, comp->name);
    if (comp->split) {
      char *exp=exp_tostring(comp->split); /* number of splits */
      coutf("    if (%sNCounter[%i] < 1000*(%s)) fprintf(stderr, \n"
        "\"Warning: Number of events %%g reaching SPLIT position Component[%i] %s\\n\"\n"
        "\"         is probably too low. Increase Ncount.\\n\", %sNCounter[%i]);\n",
          ID_PRE, comp->index, exp, comp->index, comp->name, ID_PRE, comp->index);
      str_free(exp);
    }
    coutf("    if (%sAbsorbProp[%i]) "
      "fprintf(stderr, "
        "\"Warning: %%g events were removed in Component[%i] %s\\n\"\n"
        "\"         (negative time, miss next components, rounding errors, Nan, Inf).\\n\""
        ", %sAbsorbProp[%i]);"
    , ID_PRE, comp->index, comp->index, comp->name, ID_PRE, comp->index);
  }
  list_iterate_end(liter);

  /* User's FINALLY code from the instrument definition file. */
  if(list_len(instr->finals->lines) > 0)
  {
    cout("  /* User FINALLY code from instrument definition. */");
    coutf("  SIG_MESSAGE(\"%s (Finally)\");", instr->name); /* signal handler message */
    cogen_instrument_scope(instr, (void (*)(void *))codeblock_out_brace,
                           instr->finals);
    cout("");
  }
  coutf("  %ssiminfo_close(); ", ID_PRE);
  cout("} /* end finally */");
} /* cogen_finally */

/*******************************************************************************
* cogen_mcdisplay: Generate the MCDISPLAY section.
*******************************************************************************/
static void
cogen_mcdisplay(struct instr_def *instr)
{
  List_handle liter;                /* For list iteration. */
  struct comp_inst *comp;          /* Component instance. */

  if (verbose) fprintf(stderr, "Writing instrument and components MCDISPLAY\n");

  /* User FINALLY code from component definitions (for each instance). */
  cout("#define magnify mcdis_magnify");
  cout("#define line mcdis_line");
  cout("#define dashed_line mcdis_dashed_line");
  cout("#define multiline mcdis_multiline");
  cout("#define rectangle mcdis_rectangle");
  cout("#define box mcdis_box");
  cout("#define circle mcdis_circle");
  coutf("void %sdisplay(void) {", ID_PRE);
  cout("  printf(\"MCDISPLAY: start\\n\");");
  cout("  /* Components MCDISPLAY code. */");
  cout("");

  liter = list_iterate(instr->complist);
  while(comp = list_next(liter))
  {
    if(list_len(comp->def->mcdisplay_code->lines) > 0)
    {
      char *quoted_name = str_quote(comp->name);
      coutf("  /* MCDISPLAY code for component '%s'. */", comp->name);
      coutf("  SIG_MESSAGE(\"%s (McDisplay)\");", comp->name); /* signal handler message */
      coutf("  printf(\"MCDISPLAY: component %%s\\n\", \"%s\");", quoted_name);

      cogen_comp_scope(comp, 1, (void (*)(void *))codeblock_out_brace,
                       comp->def->mcdisplay_code);
      cout("");
      str_free(quoted_name);
    }
  }
  list_iterate_end(liter);

  cout("  printf(\"MCDISPLAY: end\\n\");");
  cout("} /* end display */");
  cout("#undef magnify");
  cout("#undef line");
  cout("#undef dashed_line");
  cout("#undef multiline");
  cout("#undef rectangle");
  cout("#undef box");
  cout("#undef circle");
} /* cogen_mcdisplay */


/*******************************************************************************
* Output code for the mcstas runtime system. Default is to copy the runtime
* code into the generated executable, to minimize problems with finding the
* right files during compilation and linking, but this may be changed using
* the --no-runtime compiler switch.
*******************************************************************************/
static void
cogen_runtime(struct instr_def *instr)
{
  char *sysdir_orig;
  char *sysdir_new;
  char  pathsep[3];
  int   i,j=0;
  /* handles Windows '\' chararcters for embedding sys_dir into source code */
  if (MC_PATHSEP_C != '\\') strcpy(pathsep, MC_PATHSEP_S); else strcpy(pathsep, "\\\\");
  sysdir_orig = get_sys_dir();
  sysdir_new  = (char *)mem(2*strlen(sysdir_orig));
  for (i=0; i < strlen(sysdir_orig); i++)
  {
    if (sysdir_orig[i] == '\\')
    { sysdir_new[j] = '\\'; j++; sysdir_new[j] = '\\'; }
    else sysdir_new[j] = sysdir_orig[i];
    j++;
  }
  sysdir_new[j] = '\0';
  if(instr->use_default_main)
    cout("#define MC_USE_DEFAULT_MAIN");
  if(instr->enable_trace)
    cout("#define MC_TRACE_ENABLED");
  if(instr->portable)
    cout("#define MC_PORTABLE");
  if(instr->include_runtime)
  {
    cout("#define MC_EMBEDDED_RUNTIME"); /* Some stuff will be static. */
    embed_file("mcstas-r.h");
    /* NeXus support, only active with -DUSE_NEXUS */
    embed_file("nexus-lib.h"); /* will require linking with -DUSE_NEXUS -lNeXus */
    embed_file("nexus-lib.c");
    if (verbose) printf("Requires library    -DUSE_NEXUS -lNeXus (to enable NeXus support)\n");
    embed_file("mcstas-r.c");
  }
  else
  {
    coutf("#include \"%s%sshare%smcstas-r.h\"",  sysdir_new, pathsep, pathsep);
    coutf("#include \"%s%sshare%snexus-lib.h\"", sysdir_new, pathsep, pathsep);
    fprintf(stderr,"Dependency: %s.o\n", "mcstas-r");
    fprintf(stderr,"Dependency: %s.o and '-DUSE_NEXUS -lNeXus' to enable NeXus support\n", "nexus-lib");
    fprintf(stderr,"To build instrument %s, compile and link with these libraries (in %s%sshare)\n", instrument_definition->quoted_source, sysdir_new, pathsep);
  }

  coutf("#ifdef MC_TRACE_ENABLED");
  coutf("int %straceenabled = 1;", ID_PRE);
  coutf("#else");
  coutf("int %straceenabled = 0;", ID_PRE);
  coutf("#endif");
  coutf("#define MCSTAS \"%s%s\"", sysdir_new,pathsep);
  coutf("int %sdefaultmain = %d;", ID_PRE, instr->use_default_main);
  coutf("char %sinstrument_name[] = \"%s\";", ID_PRE, instr->name);
  coutf("char %sinstrument_source[] = \"%s\";", ID_PRE, instr->source);
  if(instr->use_default_main)
    cout("int main(int argc, char *argv[]){return mcstas_main(argc, argv);}");
} /* cogen_runtime */


/*******************************************************************************
* Generate the output file (in C).
*******************************************************************************/
void
cogen(char *output_name, struct instr_def *instr)
{
  time_t t;
  char date[64];

  time(&t);
  strncpy(date, ctime(&t), 64);
  if (strlen(date)) date[strlen(date)-1] = '\0';

  /* Initialize output file. */
  if(!output_name || !output_name[0] || !strcmp(output_name, "-"))
  {
    output_handle = fdopen(1, "w");
    quoted_output_file_name = str_quote("<stdout>");
  }
  else
  {
    output_handle = fopen(output_name, "w");
    quoted_output_file_name = str_quote(output_name);
  }
  num_next_output_line = 1;
  if(output_handle == NULL)
    fatal_error("Error opening output file '%s'\n", output_name);

  cout("/* Automatically generated file. Do not edit. ");
  cout(" * Format:     ANSI C source code");
  cout(" * Creator:    McStas <http://neutron.risoe.dk>");
  coutf(" * Instrument: %s (%s)", instr->source, instr->name);
  coutf(" * Date:       %s", date);
  cout(" */\n");
  cout("");
  coutf("#define MCSTAS_VERSION \"%s\"", MCSTAS_VERSION);
  cogen_runtime(instr);
  cogen_decls(instr);
  cogen_comp_funct(instr);
  cogen_init(instr);
  cogen_trace(instr);
  cogen_save(instr);
  cogen_finally(instr);
  cogen_mcdisplay(instr);
} /* cogen */
