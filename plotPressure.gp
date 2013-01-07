set title 'Pressure over upper surface.'
set ylabel 'Pressure (Pa)'
set xlabel 'chord (m)'
set grid
plot "sets/500/upper_surf_p.xy" using 1:4 with linespoints pi 10 title 'Upper surface.',\
     "sets/500/lower_surf_p.xy" using 1:4 with linespoints pi 10 title 'Lower surface cloud.'
