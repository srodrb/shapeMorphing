set title "Beta-capability; shape morphing and optimization."
set timestamp
set grid
set yrange [-0.4:0.4]
set xrange [-0.1:1.1]

set style line 1 linetype 12 linewidth 1 linecolor rgb "#9932CC";
set style line 2 linetype 12 linewidth 2 pointtype 64 linecolor rgb "black";

plot "splinePoints.dat" using 1:2 with lines ls 2 title "Current iteration airfoil.",\
 "controlPoints.dat" using 1:2 with points title "b-Spline control points." linecolor rgb "black",\
 "controlPoints.dat" using 1:2:3 with labels offset 0.5,0.5 notitle,\
 "coordinates.dat" using 1:2 with lines ls 1 title "Initial airfoil (upper).",\
 "coordinates.dat" using 3:4 with lines ls 1 title "Initial airfoil (lower)."


