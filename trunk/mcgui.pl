#! /usr/bin/perl -w

if($ENV{"MCSTAS"}) {
    use lib $ENV{"MCSTAS"};
} else {
    use lib "/usr/local/lib/mcstas";
}

use strict;
use Tk;

require "mcfrontlib.pl";
require "mcguilib.pl";
require "mcplotlib.pl";

my $current_sim_file;

sub is_erase_ok {
    my ($w) = @_;
    return 1;
}
    
sub menu_quit {
    my ($w) = @_;
    $w->destroy;
}

sub menu_open {
    my ($w) = @_;
    return 0 unless(is_erase_ok($w));
    my $file = $w->getOpenFile(-defaultextension => "instr",
			       -title => "Select instrument file");
}

sub menu_save {
    my ($w) = @_;
}

sub menu_saveas {
    my ($w) = @_;
}

sub menu_run {
    my ($w) = @_;
    my $file = $w->getOpenFile(-defaultextension => "sim",
			       -title => "Select simulation file");
    $current_sim_file = $file if $file && -r $file;
}

sub menu_run_simulation {
    my ($w) = @_;
    my ($ii, $si, $di) = read_sim_file($current_sim_file);

    my ($bt, $newsi) = simulation_dialog($w, $ii, $si);
    print "Pressed: '$bt'\n";
    for my $k (keys %$newsi) {
	if($k =~ /^Params$/i) {
	    for (keys %{$newsi->{$k}}) {
		print "Param: $_=$newsi->{$k}{$_}\n";
	    }
	} else {
	    print "$k: $newsi->{$k}\n";
	}
    }
}

sub menu_plot_results {
    my ($w) = @_;
    my ($ii, $si, $di) = read_sim_file($current_sim_file);

    plot_dialog($w, $ii, $si, $di);
}

sub menu_usingmcstas {
    my ($w) = @_;
}

sub menu_about {
    my ($w) = @_;
}

sub setup_menu {
    my ($w) = @_;
    my $menu = $w->Frame(-relief => 'raised', -borderwidth => 2);
    $menu->pack(-fill => 'x');
    my $filemenu = $menu->Menubutton(-text => 'File', -underline => 0);
    $filemenu->command(-label => 'Open ...',
		       -accelerator => 'Alt+O',
		       -command => [\&menu_open, $w],
		       -underline => 0);
    $w->bind('<Alt-o>' => [\&menu_open, $w]);
    $filemenu->command(-label => 'Save',
		       -accelerator => 'Alt+S',
		       -command => [\&menu_save, $w],
		       -underline => 0);
    $w->bind('<Alt-s>' => [\&menu_save, $w]);
    $filemenu->command(-label => 'Save as ...',
		       -underline => 5,
		       -command => sub {menu_saveas($w)});
    $filemenu->command(-label => 'Quit',
		       -underline => 0,
		       -accelerator => 'Alt+Q',
		       -command => [\&menu_quit, $w]);
    $w->bind('<Alt-q>' => [\&menu_quit, $w]);
    $filemenu->pack(-side=>'left');
    my $simmenu = $menu->Menubutton(-text => 'Run', -underline => 0);
    $simmenu->command(-label => 'Read parameters ...',
		       -underline => 0,
		       -command => sub {menu_run($w)});
    $simmenu->separator;
    $simmenu->command(-label => 'Run simulation ...',
		      -underline => 0,
		      -accelerator => 'Alt+U',
		      -command => sub {menu_run_simulation($w);});
    $w->bind('<Alt-u>' => [\&menu_run_simulation, $w]);
    $simmenu->command(-label => 'Plot results ...',
		      -underline => 0,
		      -accelerator => 'Alt+P',
		      -command => sub {menu_plot_results($w);});
    $w->bind('<Alt-p>' => [\&menu_plot_results, $w]);
    $simmenu->pack(-side=>'left');
    my $helpmenu = $menu->Menubutton(-text => 'Help', -underline => 0);
    $helpmenu->command(-label => 'Using McGui',
		       -underline => 6,
		       -command => sub {menu_usingmcstas($w)});
    $helpmenu->command(-label => 'About',
		       -underline => 0,
		       -command => sub {menu_about($w)});
    $helpmenu->pack(-side=>'right');
}

sub setup_edit {
    my ($w) = @_;
    my $e = $w->Text(-relief => 'sunken', -bd => '2', -setgrid => 'true',
		      -height => 30);
    my $s = $w->Scrollbar(-command => [$e, 'yview']);
    $e->configure(-yscrollcommand =>  [$s, 'set']);
    $s->pack(-side => 'right', -fill => 'y');
    $e->pack(-expand => 'yes', -fill => 'both');
    $e->insert('0.0', "");
    $e->mark('set', 'insert', '0.0');
}

# Check if simulation needs recompiling.
sub check_if_need_recompile {
    my ($simname) = @_;
    my $exename;
    if($simname =~ /^(.*)\.(instr|ins)$/) {
	$exename = $1;
    } else {
	$exename = "$simname.out";
    }
    return "not found" unless -f $exename;
    return "not executable" unless -x $exename;
    my @stat1 = stat($simname);
    my @stat2 = stat($exename);
    return "source is newer" unless $stat1[9] < $stat2[9];
    return "";
}

my $win = new MainWindow;
setup_menu($win);
setup_edit($win);

MainLoop;
