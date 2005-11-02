/*******************************************************************************
*
* McStas, neutron ray-tracing package
*         Copyright 1997-2002, All rights reserved
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
* Version: 1.24
*
* Code generation from instrument definition.
*
* $Log: not supported by cvs2svn $
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
* $Id: cogen.c,v 1.58 2005-11-02 09:18:38 farhi Exp $
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
* ##nx             Neutron state (position, velocity, time, and spin).
* ##ny
* ##nz
* ##nvx
* ##nvy
* ##nvz
* ##nt
* ##nsx
* ##nsy
* ##nsz
* ##np
* ##compcurname
* ##compcurtype
* ##compcurindex
* ##absorb          label for ABSORB (goto)
* ##Scattered       Incremented each time a SCATTER is done
* ##NCounter        Incremented each time a neutron is entering the component
* ##AbsorbProp      single counter for removed events in PROP calls
* ##comp_storein    Positions of neutron entering each comp (loc. coords)
* ##Group<GROUP>    Flag true when in an active group
* ##sig_message     Message for the signal handler (debug/trace, sim status)
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
}


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
    coutf("#define %s %sip%s", par->id, ID_PRE, par->id);
    cogen_instrument_scope_rec(parlist, func, data);
    coutf("#undef %s", par->id);
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
    (*func)(data);                /* Now do the body. */
    if(infunc == 2 && list_len(comp->extend->lines) > 0)
    {
      coutf("/* '%s' component extend code */",comp->name);
      coutf("    SIG_MESSAGE(\"%s (Trace:Extend)\");", comp->name); /* ADD: E. Farhi Aug 25th, 2002 */
      codeblock_out(comp->extend);
    }
  }
}

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
      coutf("{   /* Declarations of SETTING parameters. */");
    set = list_iterate(set_list);
    cogen_comp_scope_setpar(comp, set, infunc, func, data);
    list_iterate_end(set);
    if(infunc && list_len(set_list) > 0)
      coutf("}   /* End of SETTING parameter declarations. */");
  }
}

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
}


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
  List_handle liter;                /* For list iteration. */
  struct comp_iformal *c_formal;/* Name of component formal input parameter */
  struct instr_formal *i_formal;/* Name of instrument formal parameter. */
  struct comp_inst *comp;       /* Component instance. */
  int    index = 0;        /* ADD: E. Farhi Sep 20th, 2001 index of comp instance */
  struct group_inst *group;     /* ADD: E. Farhi Sep 24th, 2001 group instances */

  /* 1. Function prototypes. */
  coutf("void %sinit(void);", ID_PRE);
  coutf("void %sraytrace(void);", ID_PRE);
  coutf("void %ssave(FILE *);", ID_PRE);
  coutf("void %sfinally(void);", ID_PRE);
  coutf("void %sdisplay(void);", ID_PRE);
  cout("");

  if(instr->nxdinfo->any) {
    coutf("/* Init NeXus file support declarations, using the NeXus Dictionary API */");
    cout("#include \"nxdict.h\"");
    coutf("void %snxdict_init(void); ", ID_PRE);
    coutf("void %snxdict_cleanup(void); ", ID_PRE);
    coutf("void %snxdict_nxout(void); ", ID_PRE);
  }

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
  while(i_formal = list_next(liter))
  {
    coutf("%s " ID_PRE "ip%s;", instr_formal_type_names_real[i_formal->type], i_formal->id);
  }
  list_iterate_end(liter);
  cout("");

  /* 4. Table of instrument parameters. */
  coutf("#define %sNUMIPAR %d", ID_PRE, list_len(instr->formals));
  coutf("int %snumipar = %d;", ID_PRE, list_len(instr->formals));
  coutf("struct %sinputtable_struct %sinputtable[%sNUMIPAR+1] = {",
        ID_PRE, ID_PRE, ID_PRE);
  liter = list_iterate(instr->formals);
  while(i_formal = list_next(liter))
  {
    if (i_formal->isoptional && !strcmp(instr_formal_type_names[i_formal->type],"instr_type_string"))
      coutf("  \"%s\", &%sip%s, %s, %s, ", i_formal->id, ID_PRE,
          i_formal->id, instr_formal_type_names[i_formal->type],
          exp_tostring(i_formal->default_value));
    else
      coutf("  \"%s\", &%sip%s, %s, \"%s\", ", i_formal->id, ID_PRE, i_formal->id,
          instr_formal_type_names[i_formal->type],
          i_formal->isoptional ? exp_tostring(i_formal->default_value) : "");
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
  coutf("MCNUM %scomp_storein[11*%i];", ID_PRE, list_len(instr->complist)+2);

  /* 7. Table to store position (abs/rel) for each component */
  cout("/* Components position table (absolute and relative coords) */");
  coutf("Coords %scomp_posa[%i];", ID_PRE, list_len(instr->complist)+2);
  coutf("Coords %scomp_posr[%i];", ID_PRE, list_len(instr->complist)+2);
  cout("/* Counter for each comp to check for inactive ones */");
  coutf("MCNUM  %sNCounter[%i];", ID_PRE, list_len(instr->complist)+2);
  cout("/* Counter for PROP ABSORB */");
  coutf("MCNUM  %sAbsorbProp[%i];", ID_PRE, list_len(instr->complist)+2);

  /* 8. Declaration of group flags */
  cout("/* Flag true when previous component acted on the neutron (SCATTER) */");
  coutf("MCNUM %sScattered=0;", ID_PRE);

  if (list_len(instr->grouplist) > 0)
  {
    cout("/* Component group definitions (flags) */");
    liter = list_iterate(instr->grouplist);
    while(group = list_next(liter))
    {
      coutf("char %sGroup%s=0;", ID_PRE, group->name);
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
    comp->index = index;

    if(list_len(comp->def->def_par) > 0)
    {                                /* (The if avoids a redundant comment.) */
      coutf("/* Definition parameters for component '%s' [%i]. */", comp->name, index);
      liter2 = list_iterate(comp->def->def_par);
      while(c_formal = list_next(liter2))
      {
        struct Symtab_entry *entry = symtab_lookup(comp->defpar, c_formal->id);
        char *val = exp_tostring(entry->val);
        coutf("#define %sc%s_%s %s", ID_PRE, comp->name, c_formal->id, val);
        str_free(val);
      }
      list_iterate_end(liter2);
    }
    if(list_len(comp->def->set_par) > 0)
    {
      coutf("/* Setting parameters for component '%s' [%i]. */", comp->name, index);
      liter2 = list_iterate(comp->def->set_par);
      while(c_formal = list_next(liter2))
      {
        if (c_formal->type != instr_type_string)
          coutf("%s %sc%s_%s;", instr_formal_type_names_real[c_formal->type], ID_PRE, comp->name, c_formal->id);
        else  /* char type for component */
          coutf("%s %sc%s_%s[1024];", instr_formal_type_names_real[c_formal->type+1], ID_PRE, comp->name, c_formal->id);
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
    if((list_len(comp->def->decl_code->lines) > 0) || (comp->def->comp_inst_number < 0))
    {
      coutf("/* User declarations for component '%s' [%i]. */", comp->name, index);
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

  /* 12. Neutron state. */
  coutf("MCNUM %snx, %sny, %snz, %snvx, %snvy, %snvz, %snt, "
        "%snsx, %snsy, %snsz, %snp;",
        ID_PRE, ID_PRE, ID_PRE, ID_PRE, ID_PRE, ID_PRE, ID_PRE,
        ID_PRE, ID_PRE, ID_PRE, ID_PRE);
  cout("");

}


/*******************************************************************************
* Code generation for any NEXUS declarations.
*******************************************************************************/

/* this code generation comes after the 'init' code generation, thus comp
   parameters are known, and can be saved within nxdict_init */

static void
cogen_nxdict(struct instr_def *instr)
{
  List_handle liter;
  struct comp_inst *comp;
  struct instr_formal *i_formal;
  char *quoted_name = str_quote(instr->nxdinfo->nxdfile);
  /* now create Instrument parameter list to be sent to NXdict routines */

  coutf("  SIG_MESSAGE(\"%s (NeXus init)\");", instr->name); /* ADD: E. Farhi Aug 25th, 2002 */
}


/*******************************************************************************
* Generate the ##init() function.
*******************************************************************************/
static void
cogen_init(struct instr_def *instr)
{
  List_handle liter;
  struct comp_inst *comp, *last;
  char *d2r;

  coutf("void %sinit(void) {", ID_PRE);

  /* User initializations from instrument definition. */
  cogen_instrument_scope(instr, (void (*)(void *))codeblock_out_brace,
                         instr->inits);

  /* MOD: E. Farhi Sep 20th, 2001 moved transformation block so that */
  /*                              it can be used in following init block */

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
    coutf("    SIG_MESSAGE(\"%s (Init:Place/Rotate)\");", comp->name); /* ADD: E. Farhi Sep 20th, 2001 */

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
    coutf("    %sNCounter[%i]  = 0;", ID_PRE, comp->index);
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
    coutf("  SIG_MESSAGE(\"%s (Init)\");", comp->name); /* ADD: E. Farhi Sep 20th, 2001 */
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
        coutf("  if(%s) strncpy(%sc%s_%s,%s, 1024); else %sc%s_%s[0]='\\0';", val, ID_PRE, comp->name, par->id, val, ID_PRE, comp->name, par->id);
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

  if(instr->nxdinfo->any) {
    coutf("#ifdef NXDICTAPI");
    coutf("  %snxdict_init(); ", ID_PRE);
    coutf("#endif");
  }

  /* Output graphics representation of components. */
  coutf("    if(%sdotrace) %sdisplay();", ID_PRE, ID_PRE);
  coutf("    %sDEBUG_INSTR_END()", ID_PRE);
  cout("  }");
  cout("");

  cout("}");
  cout("");
}

static void
cogen_trace(struct instr_def *instr)
{
  List_handle liter;
  struct comp_inst *comp;
  struct group_inst *group;
  char   is_in_group=0;
  char  *last_group_name;

  /* Output the function header. */
  coutf("void %sraytrace(void) {", ID_PRE);

  /* Local neutron state. */
  cout("  /* Copy neutron state to local variables. */");
  coutf("  MCNUM %snlx = %snx;", ID_PRE, ID_PRE);
  coutf("  MCNUM %snly = %sny;", ID_PRE, ID_PRE);
  coutf("  MCNUM %snlz = %snz;", ID_PRE, ID_PRE);
  coutf("  MCNUM %snlvx = %snvx;", ID_PRE, ID_PRE);
  coutf("  MCNUM %snlvy = %snvy;", ID_PRE, ID_PRE);
  coutf("  MCNUM %snlvz = %snvz;", ID_PRE, ID_PRE);
  coutf("  MCNUM %snlt = %snt;", ID_PRE, ID_PRE);
  coutf("  MCNUM %snlsx = %snsx;", ID_PRE, ID_PRE);
  coutf("  MCNUM %snlsy = %snsy;", ID_PRE, ID_PRE);
  coutf("  MCNUM %snlsz = %snsz;", ID_PRE, ID_PRE);
  coutf("  MCNUM %snlp = %snp;", ID_PRE, ID_PRE);
  cout("");

  /* Debugging (initial state). */
  coutf("  %sDEBUG_ENTER()", ID_PRE);
  coutf("  %sDEBUG_STATE(%snlx, %snly, %snlz, %snlvx, %snlvy, %snlvz,"
        "%snlt,%snlsx,%snlsy, %snlp)",
        ID_PRE, ID_PRE, ID_PRE, ID_PRE, ID_PRE, ID_PRE, ID_PRE,
        ID_PRE, ID_PRE, ID_PRE, ID_PRE);

  /* ADD: E. Farhi Sep 25th, 2001 Set group flags */
  if (list_len(instr->grouplist) > 0)
  {
    cout("/* Set Component group definitions (flags) */");
    liter = list_iterate(instr->grouplist);
    while(group = list_next(liter))
    {
      coutf("  %sGroup%s=0;", ID_PRE, group->name);
    }
    list_iterate_end(liter);
  }
  coutf("#define %sabsorb %sabsorbAll", ID_PRE, ID_PRE);

  /* Now the trace code for each component. Proper scope is set up for each
     component using #define/#undef. */
  liter = list_iterate(instr->complist);
  while((comp = list_next(liter)) != NULL)
  {
    char *statepars[10];
    static char *statepars_names[10] =
      {
        "nlx", "nly", "nlz", "nlvx", "nlvy", "nlvz",
        "nlt", "nlsx", "nlsy", "nlp"
      };
    int i;
    List_handle statepars_handle;

    coutf("  /* TRACE Component %s. */", comp->name);
    coutf("  SIG_MESSAGE(\"%s (Trace)\");", comp->name); /* ADD: E. Farhi Sep 20th, 2001 */
    coutf("  %sDEBUG_COMP(\"%s\")", ID_PRE, comp->name);
    /* Change of coordinates. */
    coutf("  %scoordschange(%sposr%s, %srotr%s,", ID_PRE, ID_PRE, comp->name,
          ID_PRE, comp->name);
    coutf("    &%snlx, &%snly, &%snlz,", ID_PRE, ID_PRE, ID_PRE);
    coutf("    &%snlvx, &%snlvy, &%snlvz,", ID_PRE, ID_PRE, ID_PRE);
    coutf("    &%snlt, &%snlsx, &%snlsy);", ID_PRE, ID_PRE, ID_PRE);
    if(instr->polarised)
      coutf("  %scoordschange_polarisation("
            "%srotr%s, &%snlsx, &%snlsy, &%snlsz);",
            ID_PRE, ID_PRE, comp->name, ID_PRE, ID_PRE, ID_PRE);

    /* Debugging (entry into component). */
    coutf("  %sDEBUG_STATE(%snlx, %snly, %snlz, %snlvx, %snlvy, %snlvz,"
          "%snlt,%snlsx,%snlsy, %snlp)",
          ID_PRE, ID_PRE, ID_PRE, ID_PRE, ID_PRE, ID_PRE, ID_PRE,
          ID_PRE, ID_PRE, ID_PRE, ID_PRE);

    /* Trace code. */
    for(i = 0; i < 10; i++)
      statepars[i] = NULL;
    statepars_handle = list_iterate(comp->def->state_par);
    for(i = 0; i < 10; i++)
    {
      statepars[i] = list_next(statepars_handle);
      if(statepars[i] == NULL)
        break;
    }
    list_iterate_end(statepars_handle);
    for(i = 0; i < 10; i++)
    {
      if(statepars[i] != NULL)
        coutf("#define %s %s%s", statepars[i], ID_PRE, statepars_names[i]);
      else
        break;
    }
    if(comp->def->polarisation_par)
    {
      coutf("#define %s %s%s", comp->def->polarisation_par[0], ID_PRE, "nlsx");
      coutf("#define %s %s%s", comp->def->polarisation_par[1], ID_PRE, "nlsy");
      coutf("#define %s %s%s", comp->def->polarisation_par[2], ID_PRE, "nlsz");
    }
    /* ADD: E. Farhi Sep 20th, 2001 store neutron state in mccomp_storein */
    coutf("  STORE_NEUTRON(%i,%snlx, %snly, %snlz, %snlvx,"
          "%snlvy,%snlvz,%snlt,%snlsx,%snlsy, %snlsz, %snlp);",
          comp->index, ID_PRE, ID_PRE, ID_PRE, ID_PRE, ID_PRE, ID_PRE,
          ID_PRE, ID_PRE, ID_PRE, ID_PRE, ID_PRE, ID_PRE);
    coutf("  %sScattered=0;", ID_PRE);
    coutf("  %sNCounter[%i]++;", ID_PRE, comp->index);
    if (comp->group != NULL)
    {
      coutf("  if (!%sGroup%s)", ID_PRE, comp->group->name);
      cout("  {");
      coutf("#undef %sabsorb", ID_PRE);
      coutf("#define %sabsorb %sabsorbComp%s", ID_PRE, ID_PRE, comp->name);
      is_in_group =1;
      last_group_name = comp->group->name;
    }
    else
    if (is_in_group)
    { /* this comp is not in a group, but previous was */
      is_in_group =0;
      coutf("  /* Leave %s group thus absorb non scattered neutrons */", last_group_name);
      coutf("  if (!%sGroup%s) ABSORB;", ID_PRE, last_group_name);
    }
    cogen_comp_scope(comp, 2, (void (*)(void *))codeblock_out_brace,
                     comp->def->trace_code);
    if (comp->group != NULL)
    {
      coutf("#undef %sabsorb", ID_PRE);
      coutf("#define %sabsorb %sabsorbAll", ID_PRE, ID_PRE);
      coutf("  } /* end comp %s in group %s */", comp->name, comp->group->name);
      coutf("  if (%sScattered) %sGroup%s=%i;", ID_PRE, ID_PRE, comp->group->name, comp->index);
      cout("  /* Label to skip component instead of absorb */");
      coutf("  %sabsorbComp%s:", ID_PRE, comp->name);
      coutf("  if (!%sGroup%s)", ID_PRE, comp->group->name);
      coutf("    { RESTORE_NEUTRON(%i,%snlx, %snly, %snlz, %snlvx,"
          "%snlvy,%snlvz,%snlt,%snlsx,%snlsy, %snlsz, %snlp); }",
          comp->index, ID_PRE, ID_PRE, ID_PRE, ID_PRE, ID_PRE, ID_PRE,
          ID_PRE, ID_PRE, ID_PRE, ID_PRE, ID_PRE, ID_PRE);
    }
    if(comp->def->polarisation_par)
    {
      coutf("#undef %s", comp->def->polarisation_par[2]);
      coutf("#undef %s", comp->def->polarisation_par[1]);
      coutf("#undef %s", comp->def->polarisation_par[0]);
    }
    for(i = 9; i >= 0; i--)
    {
      if(statepars[i] != NULL)
        coutf("#undef %s", statepars[i]);
    }
    /* Debugging (exit from component). */
    coutf("  %sDEBUG_STATE(%snlx, %snly, %snlz, %snlvx, %snlvy, %snlvz,"
          "%snlt,%snlsx,%snlsy, %snlp)",
          ID_PRE, ID_PRE, ID_PRE, ID_PRE, ID_PRE, ID_PRE, ID_PRE,
          ID_PRE, ID_PRE, ID_PRE, ID_PRE);
    cout("");

  }
  list_iterate_end(liter);

  /* Absorbing neutrons - goto this label to skip remaining components. */
  coutf(" %sabsorbAll:", ID_PRE);

  /* Debugging (final state). */
  coutf("  %sDEBUG_LEAVE()", ID_PRE);
  coutf("  %sDEBUG_STATE(%snlx, %snly, %snlz, %snlvx, %snlvy, %snlvz,"
        "%snlt,%snlsx,%snlsy, %snlp)",
        ID_PRE, ID_PRE, ID_PRE, ID_PRE, ID_PRE, ID_PRE, ID_PRE,
        ID_PRE, ID_PRE, ID_PRE, ID_PRE);


  /* Copy back neutron state to global variables. */
  /* ToDo: Currently, this will be in the local coordinate system of the last
     component - should be transformed back into the global system. */
  cout("  /* Copy neutron state to global variables. */");
  coutf("  %snx = %snlx;", ID_PRE, ID_PRE);
  coutf("  %sny = %snly;", ID_PRE, ID_PRE);
  coutf("  %snz = %snlz;", ID_PRE, ID_PRE);
  coutf("  %snvx = %snlvx;", ID_PRE, ID_PRE);
  coutf("  %snvy = %snlvy;", ID_PRE, ID_PRE);
  coutf("  %snvz = %snlvz;", ID_PRE, ID_PRE);
  coutf("  %snt = %snlt;", ID_PRE, ID_PRE);
  coutf("  %snsx = %snlsx;", ID_PRE, ID_PRE);
  coutf("  %snsy = %snlsy;", ID_PRE, ID_PRE);
  coutf("  %snsz = %snlsz;", ID_PRE, ID_PRE);
  coutf("  %snp = %snlp;", ID_PRE, ID_PRE);

  /* Function end. */
  cout("}");
  cout("");
}

static void
cogen_save(struct instr_def *instr)
{
  List_handle liter;             /* For list iteration. */
  struct comp_inst *comp;        /* Component instance. */

  /* User SAVE code from component definitions (for each instance). */
  coutf("void %ssave(FILE *handle) {", ID_PRE);
  /* In case the save occurs during simulation (-USR2 not at end), we must close
   * current siminfo and re-open it, not to have redundant monitor entries
   * saved each time for each monitor. The sim_info is then incomplete, but
   * data is saved entirely. It is completed during the last siminfo_close
   * of mcraytrace
   */
  cout("  if (!handle) mcsiminfo_init(NULL);");
  cout("  /* User component SAVE code. */");
  cout("");
  liter = list_iterate(instr->complist);
  while(comp = list_next(liter))
  {
    if(list_len(comp->def->save_code->lines) > 0)
    {
      coutf("  /* User SAVE code for component '%s'. */", comp->name);
      coutf("  SIG_MESSAGE(\"%s (Save)\");", comp->name); /* ADD: E. Farhi Sep 20th, 2001 */
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
  cout("  if (!handle) mcsiminfo_close(); ");
  cout("}");
}


static void
cogen_finally(struct instr_def *instr)
{
  List_handle liter;                /* For list iteration. */
  struct comp_inst *comp;        /* Component instance. */

  /* User FINALLY code from component definitions (for each instance). */
  coutf("void %sfinally(void) {", ID_PRE);
  cout("  /* User component FINALLY code. */");
  /* first call SAVE code to save any remaining data */
  cout("  mcsiminfo_init(NULL);");
  coutf("  %ssave(%ssiminfo_file); /* save data when simulation ends */", ID_PRE, ID_PRE);
  cout("");
  liter = list_iterate(instr->complist);
  while(comp = list_next(liter))
  {
    if(list_len(comp->def->finally_code->lines) > 0)
    {
      coutf("  /* User FINALLY code for component '%s'. */", comp->name);
      coutf("  SIG_MESSAGE(\"%s (Finally)\");", comp->name); /* ADD: E. Farhi Sep 20th, 2001 */
      cogen_comp_scope(comp, 1, (void (*)(void *))codeblock_out_brace,
                       comp->def->finally_code);
      cout("");
    }
    coutf("    if (!%sNCounter[%i]) "
      "fprintf(stderr, \"Warning: No neutron could reach Component[%i] %s\\n\");",
      ID_PRE, comp->index, comp->index, comp->name);
    coutf("    if (%sAbsorbProp[%i]) "
      "fprintf(stderr, "
        "\"Warning: %%g events were removed in Component[%i] %s\\n"
        "         (negative time, rounding errors).\\n"
        "\", %sAbsorbProp[%i]);"
    , ID_PRE, comp->index, comp->index, comp->name, ID_PRE, comp->index);
  }
  list_iterate_end(liter);

  /* User's FINALLY code from the instrument definition file. */
  if(list_len(instr->finals->lines) > 0)
  {
    cout("  /* User FINALLY code from instrument definition. */");
    coutf("  SIG_MESSAGE(\"%s (Finally)\");", instr->name); /* ADD: E. Farhi Aug 25th, 2002 */
    cogen_instrument_scope(instr, (void (*)(void *))codeblock_out_brace,
                           instr->finals);
    cout("");
  }
  cout("  mcsiminfo_close(); ");
  cout("}");
}


static void
cogen_mcdisplay(struct instr_def *instr)
{
  List_handle liter;                /* For list iteration. */
  struct comp_inst *comp;        /* Component instance. */

  /* User FINALLY code from component definitions (for each instance). */
  cout("#define magnify mcdis_magnify");
  cout("#define line mcdis_line");
  cout("#define multiline mcdis_multiline");
  cout("#define circle mcdis_circle");
  coutf("void %sdisplay(void) {", ID_PRE);
  cout("  printf(\"MCDISPLAY: start\\n\");");
  cout("  /* Component MCDISPLAY code. */");
  cout("");
  liter = list_iterate(instr->complist);
  while(comp = list_next(liter))
  {
    if(list_len(comp->def->mcdisplay_code->lines) > 0)
    {
      char *quoted_name = str_quote(comp->name);
      coutf("  /* MCDISPLAY code for component '%s'. */", comp->name);
      coutf("  SIG_MESSAGE(\"%s (McDisplay)\");", comp->name); /* ADD: E. Farhi Sep 20th, 2001 */
      coutf("  printf(\"MCDISPLAY: component %%s\\n\", \"%s\");", quoted_name);
      cogen_comp_scope(comp, 1, (void (*)(void *))codeblock_out_brace,
                       comp->def->mcdisplay_code);
      cout("");
      str_free(quoted_name);
    }
  }
  list_iterate_end(liter);

  cout("  printf(\"MCDISPLAY: end\\n\");");
  cout("}");
  cout("#undef magnify");
  cout("#undef line");
  cout("#undef multiline");
  cout("#undef circle");
}


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
    embed_file("mcstas-r.c");
  }
  else
  {
    coutf("#include \"%s%sshare%smcstas-r.h\"", sysdir_new, pathsep, pathsep);
    fprintf(stderr,"Dependency: %s.o\n", "mcstas-r");
    fprintf(stderr,"To make instrument %s, compile and link with these libraries\n", instrument_definition->quoted_source);
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
}


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
  cogen_init(instr);
  if(instr->nxdinfo->any)
    cogen_nxdict(instr);        /* Only if NEXUS is actually used */
  cogen_trace(instr);
  cogen_save(instr);
  cogen_finally(instr);
  cogen_mcdisplay(instr);
}
