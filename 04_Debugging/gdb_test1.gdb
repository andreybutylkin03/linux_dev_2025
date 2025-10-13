set pagination off

b range.c:22 if idx % 5 == 0
command 1
    silent
    printf "@@@ %d %d %d %d\n", beg, end, step_val, idx
    continue
end

run 1 12
quit
