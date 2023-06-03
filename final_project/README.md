# Final Project

| Implemantation | clock period (ns) | Area   | Avg. Latency (cycles) | Total Runtime (ns) | Description |
| -------------- | ----------------- | ------ | --------------------- | ------------------ | ----------- | 
| BASIC          | 10                | 3843.2 | 2002                  | 20040010           | 1000 signals in RAM_1000X8 | 
| MEM_1000X2     | 10                | 4749.2 | 1002                  | 10040010           | using 2 RAM_1000X8, both storing 1000 signal |
| MEM_500X2      | 10                | 5675.3 |  752                  |  7540030           | using 2 RAM_500X8, both storing 500 signal |
| MEM_250X4      | 10                | 9969.6 |  377                  |  3790070           | using 4 RAM_250X8, all storing 250 signal (4 loops) |
| MEM_250X4_AREA | 10                | 7999.7 |  380                  |  3812570           | using 4 RAM_250X8, all storing 250 signal (2 loops only) |