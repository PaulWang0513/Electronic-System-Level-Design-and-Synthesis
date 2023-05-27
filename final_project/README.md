# Final Project

| Implemantation | clock period (ns) | Area   | Avg. Latency (cycles) | Total Runtime (ns) | Description |
| -------------- | ----------------- | ------ | --------------------- | ------------------ | ----------- | 
| BASIC          | 10                | 3467.8 | 2002                  | 20049990           | 1000 signals in RAM_1000X8 | 
| MEM_1000X2     | 10                | 4402.9 | 2002                  | 20049990           | 2 RAM_1000X8 both storing 1000 signal |
| MEM_500X2      | 10                | 5086.2 | 1502                  | 15049990           | 2 RAM_500X8 both storing 500 signal |

- Waiting to try
    - 2 memories, each storing 1000 8-bit signals => 2 times faster in reading data
    - 2 memories, each storing 500 8-bit signals => only half of the data can be parellelly read
    - more memories and seperate the control flow