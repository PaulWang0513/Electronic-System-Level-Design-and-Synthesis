# Final Project

| Implemantation | Description | Area | Avg. Latency (cycles) | Total Runtime (ns) |
| -------------- | ----------- | ---- | --------------------- | ------------------ |
| BASIC          | 1000 signals in RAM_1000X8 | 4443.6 | 1503 | 15064990 |

- Waiting to try
    - 2 memories, each storing 1000 8-bit signals => 2 times faster in reading data
    - 2 memories, each storing 500 8-bit signals => only half of the data can be parellelly read
    - more memories and seperate the control flow