set term gif animate optimize delay 5 size 480,360
set output 'path.gif'

set grid
set xrange[-3:3]
set xlabel "x"
set ylabel "t"
unset key


do for [ i = 1000:1000000:1000 ]{
   set title "MCstep ".i
   plot "data0MCstep".i.".txt" w lp pt 7
}

set out
