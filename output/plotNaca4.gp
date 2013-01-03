set timestamp
set title ""
set grid
set yrange [-0.4:0.4]
set xrange [-0.1:1.1]

set style line 2 linetype 12 linewidth 1 pointtype 64 linecolor rgb "black";

plot "coordinates.dat" using 1:2 with lines ls 2 title "upper surface",\
     "coordinates.dat" using 3:4 with lines title "lower surface"
