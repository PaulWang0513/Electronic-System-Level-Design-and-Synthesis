# Final Project

| Implemantation | clock period (ns) | Area   | Avg. Latency (cycles) | Total Runtime (ns) | Description |
| -------------- | ----------------- | ------ | --------------------- | ------------------ | ----------- | 
| BASIC          | 10                | 3843.2 | 2002                  | 20040010           | 1000 signals in RAM_1000X8 | 
| MEM_1000X2     | 10                | 4749.2 | 1002                  | 10040010           | 2 RAM_1000X8 both storing 1000 signal |
| MEM_500X2      | 10                | 5736.3 | 1002                  | 10035030           | 2 RAM_500X8 both storing 500 signal |
| MEM_250X4      | 10                | 9969.6 |  377                  |  3790070           | 4 RAM_250X8 both storing 250 signal (4 loops) |
| MEM_250X4_AREA | 10                | 7999.7 |  380                  |  3812570           | 4 RAM_250X8 both storing 250 signal (2 loops only) |

- Waiting to try
    - 2 memories, each storing 1000 8-bit signals => 2 times faster in reading data (same with expected)
    - 2 memories, each storing 500 8-bit signals => use different pattern for front and end 500 signals (slower than expected, because the front part cannot do so fast)
    - 4 memories, each storing 250 8-bit signals