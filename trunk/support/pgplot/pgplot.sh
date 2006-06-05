#!/bin/sh
#
# Simple script for setting up PGPLOT on Linux
# contains a patch for malloc/gcc error

# requires: g77 and perl
# install pgplot 5.2
ROOT_SRC=$PWD
echo tar xzf pgplot5.2.tar.gz
gunzip -c pgplot5.2.tar.gz | tar xf -
# add patch for pgdisp/malloc error
cp pgdispd/* pgplot/pgdispd/
cd pgplot
PGPLOT_SRC=$PWD
PGPLOT_DIR="/usr/local/pgplot/"; export PGPLOT_DIR
mkdir $PGPLOT_DIR
echo cp drivers.list $PGPLOT_DIR/drivers.list
cp drivers.list $PGPLOT_DIR/drivers.list
echo cd $PGPLOT_DIR
cd $PGPLOT_DIR
sed 's/! GIDRIV/  GIDRIV/' drivers.list > drivers1.list
sed 's/! PSDRIV/  PSDRIV/' drivers1.list > drivers2.list
sed 's/! PPDRIV/  PPDRIV/' drivers2.list > drivers3.list
sed 's/! X2DRIV/  X2DRIV/' drivers3.list > drivers4.list
sed 's/! XWDRIV/  XWDRIV/' drivers4.list > drivers.list
\rm drivers?.list
echo $PGPLOT_SRC/makemake $PGPLOT_SRC linux   g77_gcc
$PGPLOT_SRC/makemake $PGPLOT_SRC linux   g77_gcc
sed 's/-lbsd//' makefile > makefile.new
cp makefile.new makefile
\rm makefile.new
echo make; make cpg
make
make cpg
make pgplot.html
make clean
# create links
ln -s $PGPLOT_DIR/pgdisp /usr/local/bin/pgdisp
ln -s $PGPLOT_DIR/pgxwin_server /usr/local/bin/pgxwin_server

# install F77
cd $ROOT_SRC
echo tar xzf ExtUtils-F77-1.13.tar.gz
gunzip -c ExtUtils-F77-1.13.tar.gz | tar xf -
cd ExtUtils-F77-1.13
echo perl Makefile.PL; make; make instal
perl Makefile.PL
make
make install

# install perl-PGPLOT
cd $ROOT_SRC
echo tar xzf PGPLOT-2.18.tar.gz
gunzip -c PGPLOT-2.18.tar.gz | tar xf -
cd PGPLOT-2.18
echo perl Makefile.PL; make; make instal
perl Makefile.PL
make
make install

cd $ROOT_SRC
# end of pgplot install script
