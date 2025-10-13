set pagination off
set $counter = 0

b range.c:22
command 1
    silent
    set $counter = $counter + 1
    if $counter >= 28 && $counter <= 35
        printf "@@@ %d %d %d %d\n", beg, end, step_val, idx
    end
    continue
end

run -100 100 3
quit
