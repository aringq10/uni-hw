# measuring

compile with -O1

run `perf stat -e cycles ./prog` to get cpu cycles, and then divide by REPS*COUNT

# results

REPS 1000000
COUNT 16000

my_memset:
  2,017,141,690
  cpe 0.126071356
basic_memset:
  16,069,656,438
  cpe 1.004353527
my_memset1:
  2,052,604,957
  cpe 0.12828781

---

REPS 1000
COUNT 1000000

my_memset:
  166,872,857
  cpe 0.166872857
basic_memset:
  1,109,548,425
  cpe 1.109548425
my_memset1:
  160,222,976
  cpe 0.160222976
