#! /usr/bin/perl -w
#
# Generates HTML documentation from McStas component/instrument files
#
#   This file is part of the McStas neutron ray-trace simulation package
#   Copyright (C) 1997-2003, All rights reserved
#   Risoe National Laborartory, Roskilde, Denmark
#   Institut Laue Langevin, Grenoble, France
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

use FileHandle;
use Config;

# Determine the path to the McStas system directory. This must be done
# in the BEGIN block so that it can be used in a "use lib" statement
# afterwards.

use Config;
BEGIN {
    if($ENV{"MCSTAS"}) {
      $MCSTAS::sys_dir = $ENV{"MCSTAS"};
    } else {
      if ($Config{'osname'} eq 'MSWin32') {
        $MCSTAS::sys_dir = "c:\\mcstas\\lib";
      } else {
        $MCSTAS::sys_dir = "/usr/local/lib/mcstas";
      }
    }
    $MCSTAS::perl_dir = "$MCSTAS::sys_dir/tools/perl";
}

use lib $MCSTAS::perl_dir;
require "mcstas_config.perl";
require "mcrunlib.pl";

my $is_single_file= 0;  # true when doc requested for a single component
my $is_user_lib   = 0;  # true when doc requested for a directory
my $lib_dir       = $MCSTAS::sys_dir; 
my $out_file      = "index.html"; # default name for output of catalog
my $show_html     = 0;  # true when requesting to display HTML doc
my $use_local     = 0;  # true when also looking into current path
my $single_comp_name = 0;  # component single name
my $browser       = defined($ENV{'BROWSER'}) ? $ENV{'BROWSER'} : "text";
my $is_forced     = 0; # true when force re-writting of existing HTML

sub show_header {
    my ($d) = @_;
    my ($i);
    print "######## Component: $d->{'name'} #####################\n";
    print "[Author]: $d->{'identification'}{'author'}\n";
    print "[Origin]: $d->{'identification'}{'origin'}\n";
    print "[Date]: $d->{'identification'}{'date'}\n";
    print "[Version]: $d->{'identification'}{'version'}\n";
    for $i (@{$d->{'identification'}{'history'}}) {
        print "[Modified by]: $i\n";
    }
    print "\n";
    print $d->{'identification'}{'short'};
    print "######## Input parameters: ##############################\n";
    for $i (@{$d->{'inputpar'}}) {
        if(defined($d->{'parhelp'}{$i}{'default'})) {
            print "<$i=$d->{'parhelp'}{$i}{'default'}>: ";
        } else {
            print "<$i>: ";
        }
        if($d->{'parhelp'}{$i}) {
            print "[$d->{'parhelp'}{$i}{'unit'}] "
                if $d->{'parhelp'}{$i}{'unit'};
            print "$d->{'parhelp'}{$i}{'text'}"
                if $d->{'parhelp'}{$i}{'text'};  # text finishes by \n
            print("\n");
        } else {
            print("<Undocumented>\n");
        }
    }
    print "\n######## Output parameters: #############################\n";
    for $i (@{$d->{'outputpar'}}) {
        print "<$i>: ";
        if($d->{'parhelp'}{$i}) {
            print "[$d->{'parhelp'}{$i}{'unit'}] "
                if $d->{'parhelp'}{$i}{'unit'};
            print "$d->{'parhelp'}{$i}{'text'}"
                if $d->{'parhelp'}{$i}{'text'};  # text finishes by \n
            print("\n");
        } else {
            print("<Undocumented>\n");
        }
    }
    if($d->{'description'}) {
        print "\n######## Description: ###################################\n";
        print $d->{'description'};
    }
    print "\n#########################################################\n";
}

#
# Output the start of the main component index HTML table
# parameters: ($filehandle, $toolbar);
sub html_main_start {
    my ($f, $toolbar) = @_;
    print $f <<END;
<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2//EN\">
<HTML>
<HEAD>
   <META NAME="GENERATOR" CONTENT="McDoc">
   <TITLE>McStas : Library components</TITLE>
</HEAD>
<BODY>

$toolbar
<CENTER><H1>Components for <i>McStas</i></H1></CENTER>

<P> Names in <B>Boldface</B> denote components that are properly
documented with comments in the source code.</P>
END
}

#
# Output the HTML table row describing component with information in
# $d to file handle $f.
# parameters: ($data, $filehandle, $basename)
sub html_table_entry {
    my ($d, $f, $bn, $vn) = @_;
    print $f "<TR>\n";
    print $f "<TD> ";
    print $f "<B>" if %{$d->{'parhelp'}};
    print $f "<A HREF=\"$bn.comp\">$d->{'name'}</A>";
    print $f "</B>" if %{$d->{'parhelp'}};
    print $f "</TD>\n";

    print $f "<TD>$d->{'identification'}{'origin'}</TD>\n";

    print $f "<TD>$d->{'identification'}{'author'}</TD>\n";

    print $f "<TD>";
    print $f "<A HREF=\"$vn.html\">More...</A>";
    print $f "</TD>\n";

    print $f "<TD>$d->{'identification'}{'short'}</TD>\n";
    print $f "</TR>\n\n";
}

#
# Output the end of the main component index HTML table
# parameters: ($filehandle, $toolbar);
sub html_main_end {
    my ($f, $toolbar) = @_;
    my $date;
    if ($Config{'osname'} eq 'MSWin32') {
      $date = `date /T`;
    } else {
      $date = `date +'%b %e %Y'`;
    }
    print $f <<END;
<P>This Component list was updated on $date.
<HR WIDTH="100%">
<CENTER>
  [ <A HREF="http://www.ill.fr/tas/mcstas/mcstas_ill.html"><I>McStas</I> at ILL</A>
  | <A href="http://neutron.risoe.dk/mcstas/"><I>McStas</I> at Ris&oslash;</A> ]
</CENTER>

<P><BR>
<ADDRESS>
Generated by McDoc, 
Maintained by Emmanuel Farhi &lt;<a href="mailto:farhi\@ill.fr">farhi\@ill.fr</a>>
and Peter Willendrup &lt;<a href="mailto:peter.willendrup\@risoe.dk">peter.willendrup\@risoe.dk</a>>.
Contact us for any comments.
</ADDRESS>
</BODY></HTML>
END
}

#
# Output the HTML table for either input or output parameters.
#
sub gen_param_table {
    my ($f, $ps, $qs) = @_;
    my $i;
    # Avoid outputting empty table.
    unless(@$ps) {
        print $f "None.\n";
        return;
    }
    print $f "<TABLE BORDER=1>\n";
    print $f "<TR><TH>Name</TH>  <TH>Unit</TH>  <TH>Description</TH> <TH>Default</TH></TR>\n";
    for $i (@$ps) {
        my $default = $qs->{$i}{'default'};
        print $f "<TR> <TD>";
        print $f "<B>" unless defined($default);
        print $f "$i";
        print $f "</B>" unless defined($default);
        print $f "</TD>\n";
        if($qs->{$i}{'unit'} && $qs->{$i}{'text'}) {
            print $f "     <TD>$qs->{$i}{'unit'}</TD>\n";
            print $f "     <TD>$qs->{$i}{'text'}</TD>\n";
        } else {
            print $f "     <TD></TD> <TD></TD>\n";
        }
        print $f "<TD ALIGN=RIGHT>", defined($default) ?
            $default : "&nbsp;", "</TD> </TR>\n";
    }
    print $f "</TABLE>\n\n";
}

#
# Generate description web page from component with information in $d.
# parameters: ($data, $basename, $name);
sub gen_html_description {
    my ($d, $bn, $n) = @_;
    my $f = new FileHandle;
    my $toolbar = <<'TB_END';
<P ALIGN=CENTER>
 [ <A href="#id">Identification</A>
 | <A href="#desc">Description</A>
 | <A href="#ipar">Input parameters</A>
 | <A href="#opar">Output parameters</A>
 | <A href="#links">Links</A> ]
</P>
TB_END
    my $is_opened = 0;
    my $valid_name= "";
    $n=~ s|.comp\Z||; # remove trailing extension
    $n=~ s|.cmp\Z||; # remove trailing extension
    $n=~ s|.com\Z||; # remove trailing extension
    $valid_name = $bn;
    if (open($f, ">$bn.html")) { # use component location
      $is_opened = 1; 
    }
    if (((not $is_opened) && $is_forced) || (not -f "$valid_name.html")) {
      if (open($f, ">$n.html")) { # create locally
        $is_opened = 1; 
        $valid_name = $n;
      }
    }
    if ($is_single_file) { 
      $out_file = "$valid_name.html"; 
    }
    if ($is_opened) {
      print $f "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2//EN\">\n";
      print $f "<HTML><HEAD>\n";
      print $f "<TITLE>$d->{'name'}</TITLE>\n";
      print $f "<LINK REV=\"made\" HREF=\"mailto:peter.willendrup\@risoe.dk\">\n";
      print $f "</HEAD>\n\n";
      print $f "<BODY>\n\n$toolbar\n";
      print $f "<H1>The <CODE>$d->{'name'}</CODE> component</H1>\n\n";
      print $f "$d->{'identification'}{'short'}\n\n";
      print $f "<H2><A NAME=id></A>Identification</H2>\n";
      print $f "\n<UL>\n";
      print $f "  <LI> <B>Author:</B>$d->{'identification'}{'author'}</B>\n";
      print $f "  <LI> <B>Origin:</B>$d->{'identification'}{'origin'}</B>\n";
      print $f "  <LI> <B>Date:</B>$d->{'identification'}{'date'}</B>\n";
      print $f "  <LI> <B>Version:</B>$d->{'identification'}{'version'}</B>\n";
      if(@{$d->{'identification'}{'history'}}) {
          my $entry;
          print $f "  <LI> <B>Modification history:</B> <UL>\n";
          for $entry (@{$d->{'identification'}{'history'}}) {
              print $f "    <LI> $entry\n";
          }
          print $f "  </UL>\n";
      }
      print $f "</UL>\n";
      if($d->{'description'}) {
          print $f "<H2><A NAME=desc></A>Description</H2>\n";
          print $f "\n<PRE>\n$d->{'description'}</PRE>\n";
          if ($bn =~ m/obsolete/i || $n =~ m/obsolete/i) {
            print $f "WARNING: <B>This is an obsolete component.";
            print $f "Please avoid usage whenever possible.</B>\n";
          }
      }
      print $f "\n<H2><A NAME=ipar></A>Input parameters</H2>\n";
      if(@{$d->{'inputpar'}}) {
          print $f "Parameters in <B>boldface</B> are required;\n";
          print $f "the others are optional.\n";
      }
      gen_param_table($f, $d->{'inputpar'}, $d->{'parhelp'}); 
      print $f "\n<H2><A NAME=opar></A>Output parameters</H2>\n";
      gen_param_table($f, $d->{'outputpar'}, $d->{'parhelp'}); 
      print $f "\n<H2><A NAME=links></A>Links</H2>\n\n<UL>\n";
      print $f "  <LI> <A HREF=\"$d->{'name'}.comp\">Source code</A> ";
      print $f "for <CODE>$d->{'name'}.comp</CODE>.\n";
      # Additional links from component comment header go here.
      my $link;
      for $link (@{$d->{'links'}}) {
          print $f "  <LI> $link";
      }
      print $f "</UL>\n";
      print $f "<HR>\n$toolbar\n<ADDRESS>\n";
      print $f "Generated automatically by McDoc, Peter Willendrup\n";
      print $f "&lt;<A HREF=\"mailto:peter.willendrup\@risoe.dk\">";
      print $f   "peter.willendrup\@risoe.dk</A>&gt; /\n";
      if ($Config{'osname'} eq 'MSWin32') {
	print $f `date /T`;
      } else {
	print $f `date +'%b %e %Y'`;
      }
      print $f "</ADDRESS>\n";
      print $f "</BODY></HTML>\n";
      close $f;
    } else { 
      if (not -f "$valid_name.html") {
        print "mcdoc: Cannot open $valid_name.html. Use -f option to force.\n";
      }
    }
    return $valid_name;
}

#
# Add component with info in $d to web page handle $f, and generate
# stand-alone documentation page. $bn is the base name (file name
# without trailing .comp).
# parameters: ($data, $filehandle, $basename, $name);
sub add_comp_html {
    my ($d, $f, $bn, $n) = @_;
    my $vn;
    $vn = gen_html_description($d, $bn, $n);
    if ($f) { html_table_entry($d, $f, $bn, $vn); }
    
}

#
# Add a whole section of components, given the section directory name.
# parameters: ($lib_dir, $section, $section_header, $filehandle);
sub add_comp_section_html {
    my ($lib, $sec, $header, $filehandle) = @_;
    my $sec_orig = $sec;
    if ($sec =~ "local") { $sec = "."; $is_forced=1; }  # local components
    $sec = "$lib/$sec" unless -d $sec;
    if(opendir(DIR, $sec)) {
        my @comps = readdir(DIR);
        closedir DIR;
        return unless @comps;
        if ($filehandle) {
          print $filehandle <<END;

<P><A NAME="$sec_orig"></A>
$header
<TABLE BORDER COLS=5 WIDTH="100%" NOSAVE>
<TR>
<TD><B><I>Name</I></B></TD>
<TD WIDTH="10%"><B><I>Origin</I></B></TD>
<TD WIDTH="10%"><B><I>Author(s)</I></B></TD>
<TD WIDTH="10%"><B><I>Help</I></B></TD>
<TD><B><I>Description</I></B></TD>
</TR>
END
        } # end if filehandle
        my ($comp, $name);
        for $name (sort(@comps)) {
            my $comp = "$sec/$name";
            next unless $comp =~ /^(.*)\.(com|comp|cmp)$/;
            my $does_match = 0;
            my $basename = $1;
            if ($single_comp_name =~ /^(.*)\.(com|comp|cmp)$/) {
              if($name eq $single_comp_name) { $does_match = 2; }
            } elsif ($name =~ $single_comp_name) { $does_match = 1; }
            if (($is_single_file && $does_match) 
              || (not $is_single_file)) {
              $data = component_information($comp);
              if (not defined($data)) {
                print STDERR "mcdoc: Failed to get information for component '$comp'";
              } else {
                print STDOUT "mcdoc: $comp\n";
                if ($is_single_file && $show_html && $browser =~ "text") {
                  show_header($data); # display single comp as text
                  if ($sec =~ m/obsolete/i) {
                    print "WARNING: This is an obsolete component. \n";
                    print "         Please avoid usage whenever possible.\n";
                  }
                } else {
                  add_comp_html($data, $filehandle, $basename, $name);
                }
              }
              last if $does_match == 2;
            }
        } # end for
        if ($filehandle) {
          print $filehandle <<END;
</TABLE>

END
        } # end if filehandle
    } #end if open DIR
}

# Start of main ===============================

my $index         = 0;
my $file;
my $show_website  = 0;
my $show_manual   = 0;
     
for($i = 0; $i < @ARGV; $i++) {
  $_ = $ARGV[$i];
  # Options specific to mcdoc.
  if(/^--show$/i || /^-s$/i || /^--html$/i) {
        $show_html = 1;
  } elsif(/^--text$/i || /^-t$/i) {
        $show_html = 1; $browser = "text";
  } elsif(/^--web$/i || /^-w$/i) {
        $show_website = 1;
  } elsif(/^--manual$/i || /^-m$/i) {
        $show_manual = 1;
  } elsif(/^--local$/i) {
        $use_local = 1;
  } elsif(/^--force$/i || /^-f$/i) {
        $is_forced = 1;
  } elsif(/^--help$/i || /^-h$/i || /^-v$/i) {
      print "Usage: mcdoc [options] <dir|file>\n";
      print "   -f    --force   Force re-writting of existing HTML doc locally\n";
      print "   -h    --help    Show this help\n";
      print "   -l    --tools   Display the McStas tools list\n";
      print "   -m    --manual  Open the McStas manual\n";
      print "   -s    --show    Open the generated help file using the BROWSER env. variable\n";
      print "   -t    --text    For single component, display as text\n";
      print "   -w    --web     Open the McStas web page http://neutron.risoe.dk/mcstas/\n";
      print "SEE ALSO: mcstas, mcdoc, mcplot, mcrun, mcgui, mcresplot, mcstas2vitess\n";
      print "DOC:      Please visit http://neutron.risoe.dk/mcstas/\n";
      exit;
  } elsif(/^--tools$/i || /^-l$/) {
      print "McStas Tools\n";
      print "   mcstas        Main instrument compiler\n";
      print "   mcrun         Instrument maker and execution utility\n";
      print "   mcgui         Graphical User Interface instrument builder\n";
      print "   mcdoc         Component library documentation generator/viewer\n";
      print "   mcplot        Simulation result viewer\n";
      print "   mcdisplay     Instrument geometry viewer\n";
      print "   mcresplot     Instrument resolution function viewer\n";
      print "   mcstas2vitess McStas to Vitess component translation utility\n";
      print "When used with the -h flag, all tools display a specific help.\n";
      print "SEE ALSO: mcstas, mcdoc, mcplot, mcrun, mcgui, mcresplot, mcstas2vitess\n";
      print "DOC:      Please visit http://neutron.risoe.dk/mcstas/\n";
      exit;
  } else {
      $file = $ARGV[$i];
      $index++;
  }
} # end for

if ($show_website) {
  if ($browser =~ "text") {
    die "mcdoc: Set the BROWSER environment variable first\n";
  } else {
    # open the index.html
    print "mcdoc: Starting $browser http://neutron.risoe.dk/mcstas/\n";
    system("$browser http://neutron.risoe.dk/mcstas/ \n");
    die "mcdoc: web site done.\n";
  }
}

if ($show_manual) {
  if ($browser =~ "text") {
    die "mcdoc: Set the BROWSER environment variable first\n";
  } else {
    # open the index.html
    print "mcdoc: Starting $browser $MCSTAS::sys_dir/doc/mcstas-manual.pdf\n";
    system("$browser $MCSTAS::sys_dir/doc/mcstas-manual.pdf\n");
    die "mcdoc: manual done.\n";
  }
}
  
# if 'file' is given
if ($index > 0) { 
  if (-d $file) { $lib_dir = $file; } # get doc of the given dir
  else { $is_single_file=1; $single_comp_name = $file; $show_html = 1; }  # search locally and in lib
  $use_local=1; # will also search locally
}

my $filehandle = 0;
my @sections;
my %section_headers;

if (not $is_single_file) {
  # Open the local documentation file
  $filehandle = new FileHandle;
  my $no_lib_write  = 0;
  
  if (not open($filehandle, ">$lib_dir/$out_file")) { $no_lib_write = 1; }
  if ($no_lib_write) { 
    my $no_local_write = 0;
    if (not open($filehandle, ">$out_file")) { $no_local_write = 1; }
    if ($no_local_write) {
      $filehandle = 0; # will not write the catalog
      print STDERR "mcdoc: Could not open $out_file for writing.\n";
    }
  } else { 
    $out_file = "$lib_dir/$out_file";
  }
  if (not $filehandle) {
    if (-f "$lib_dir/$out_file") { $out_file = "$lib_dir/$out_file"; }
    elsif (not -f $out_file) {
      print STDERR "mcdoc: Could not find the $out_file library catalog.\n";
    }
  }
}

if ($use_local) {
  # define local and lib sections
  @sections = ("sources", "optics", "samples", "monitors", 
               "misc", "contrib", "obsolete","local","data","share");
  %section_headers =
    ("sources" => '<B><FONT COLOR="#FF0000">Sources</FONT></B>',
     "optics" => '<B><FONT COLOR="#FF0000">Optics</FONT></B>',
     "samples" => '<B><FONT COLOR="#FF0000">Samples</FONT></B>',
     "monitors" => '<B><FONT COLOR="#FF0000">Detectors</FONT> and monitors</B>',
     "contrib" => '<B><FONT COLOR="#FF0000">Contributed</FONT> components</B>',
     "misc" => '<B><FONT COLOR="#FF0000">Misc</FONT></B>',
     "obsolete" => '<B><FONT COLOR="#FF0000">Obsolete</FONT> (avoid usage whenever possible)</B>',
     "local" => '<B><FONT COLOR="#FF0000">Local components</FONT></B>',
     "data" => '<B><FONT COLOR="#FF0000">Data files</FONT></B>',
     "share" => '<B><FONT COLOR="#FF0000">Shared libraries</FONT></B>');
} else {
  # define lib sections
  @sections = ("sources", "optics", "samples", "monitors", "misc", "contrib");
  %section_headers =
    ("sources" => '<B><FONT COLOR="#FF0000">Sources</FONT></B>',
     "optics" => '<B><FONT COLOR="#FF0000">Optics</FONT></B>',
     "samples" => '<B><FONT COLOR="#FF0000">Samples</FONT></B>',
     "monitors" => '<B><FONT COLOR="#FF0000">Detectors</FONT> and monitors</B>',
     "contrib" => '<B><FONT COLOR="#FF0000">Contributed</FONT> components</B>',
     "misc" => '<B><FONT COLOR="#FF0000">Misc</FONT></B>',
     "obsolete" => '<B><FONT COLOR="#FF0000">Obsolete</FONT> (avoid usage whenever possible)</B>');
}
my @tblist = map "<A href=\"#$_\">$_</A>", @sections;
my $toolbar = "<P ALIGN=CENTER>\n [ " . join("\n | ", @tblist) . " ]\n</P>\n";

if ($filehandle) {
  html_main_start($filehandle, $toolbar);
}
# open each section, look for comps, add entry in index.html, 
# and generate comp doc
my $sec;
my $is_forced_orig = $is_forced;
for $sec (@sections) {
    add_comp_section_html($lib_dir, $sec, $section_headers{$sec}, $filehandle);
    $is_forced = $is_forced_orig; # may have been changed globally (sec == local)
}
if ($filehandle) {
  html_main_end($filehandle, $toolbar);
  close($filehandle);
}
if ($show_website) {
  if ($browser =~ "text") {
    die "mcdoc: Set the BROWSER environment variable first\n";
  } else {
    # open the index.html
    system("$browser http://neutron.risoe.dk/mcstas/ \n");
  }
}

if ($show_html && -f $out_file) {
  if ($browser ne "text") {
    # open the index.html
    print "mcdoc: Starting $browser $out_file\n";
    system("$browser $out_file \n");
  } else {
    if (not $is_single_file) { 
      die "mcdoc: Set the BROWSER environment variable to view $out_file\n"; 
    }
  }
}
