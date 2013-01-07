set title 'Residuals vs. Iterations'
set grid
set timestamp
set logscale y
set ylabel 'Residual'
set xlabel 'Iteration'
plot "< cat log | grep 'Solving for Ux' | cut -d' ' -f9 | tr -d ','" title 'Ux' with lines,\
     "< cat log | grep 'Solving for Uz' | cut -d' ' -f9 | tr -d ','" title 'Uz' with lines,\
     "< cat log | grep 'Solving for p' | cut -d' ' -f9 | tr -d ','" title 'p' with lines,\
     "< cat log | grep 'Solving for nuTilda' | cut -d' ' -f9 | tr -d ','" title 'nuTilda' with lines
reread