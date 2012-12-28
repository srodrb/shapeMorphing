set timestamp
set title "Airfoil plot"
set grid
set yrange[-0.3:0.3]
set xrange[0.0:1.0]
plot "coordinates" using 1:2 with linespoint title "upper surface",\
     "coordinates" using 3:4 with linespoint title "lower surface"
