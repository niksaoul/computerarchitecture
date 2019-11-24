#### 1. Basic simulation parameters
Most important parameters, as described in `starter_se.py` and `devices.py` are:
* Type of CPU : MinorCPU
* CPU Frequency: 4GHz
* Number of CPU Cores: 1
* Memory type : DDR3_1600_8x8
* Number of Memory channels : 2
* Number of Memory ranks / channel : None (_meaning that it doesn't take a default value_)
* Physical Memory size: 2GB
* Caches
  * L1 Data Cache: 32kB
  * L1 Instruction Cache: 48kB
  * WalkCache: 1kB
  * L2 Cache: 1MB

Every parameter, except for _Type of CPU_ which was declared as _MinorCPU_, has the default value, as defined in starte_se.py.

#### 2. Validation of the simulation parameters
###### The parameters values can be validated by searching in `config.ini`.
* Type of CPU was indeed set to MinorCPU, as we can see in the following lines.  
```
66: [system.cpu_cluster.cpus]   
67: type=MinorCPU
```
* CPU Frequency (time is measured in ps)
```
58: [system.cpu_cluster.clk_domain]
59: type=SrcClockDomain
60: clock=250
```
* Number of CPU Cores
```
171: numThreads=1
```

* Memory type  
```
1334: IDD0=0.055
1335: IDD02=0.0
...
1354: IDD6=0.02
1355: IDD62=0.0
```
```
1398: tCCD_L=0
1399: tCCD_L_WR=0
...
1417: tXS=270000
1418: tXSDLL=0
```
By comparing the lines above (from `config.ini`) with [this document](https://github.com/CSL-KU/SpectreGuard/blob/master/gem5/src/mem/DRAMCtrl.py), we were able to identify the type of Memory, which is indeed _DDR3_1600_8x8_.
* Number of Memory channels
```
1271: channels=2
```
* Ranks per channel
```
1301: ranks_per_channel=2
```
* L1 Instruction Cache
```
899: size=49152
```
* L2 Data Cache
```
197: size=32768
```
* L2 Cache
```
1151: size=1048576
```
* WalkCache
```
318: size=1024
```
*
#### 3. In order CPUs
"In-order" refers to in-order instruction execution, meaning that instructions are executed in the order they "arrive" to the CPU.
 * ##### Minor CPU model
Minor is an in-order processor model with a 4 stage fixed pipeline but configurable data structures and execute behavior. The 4 stages are:  
  * fetch1
  * fetch2
  * decode
  * execute

  It is intended to be used to model processors with strict in-order execution behavior and allows visualization of an instruction's position in the pipeline through the MinorTrace/minorview.py format/tool. The intention is to provide a framework for micro-architecturally correlating the model with a particular, chosen processor with similar capabilities.
The model **isn't currently capable of multithreading** but there are THREAD comments in key places where stage data needs to be arrayed to support multithreading.  
Information was found [here](www.gem5.org/docs/html/minor.html) and [here](https://nitish2112.github.io/post/gem5-minor-cpu/?fbclid=IwAR0rskGeqQd_bbNh11TNbfjJcAUT6g3vTP7JguEPi1YpU9QBtlYCQ2mABtE).

 * ##### Simple CPU
 The SimpleCPU is a purely functional, in-order model that is suited for cases where a detailed model is not necessary. This can include warm-up periods, client systems that are driving a host, or just testing to make sure a program works.
  * ###### Base Simple CPU
    * The BaseSimpleCPU serves several purposes:
    * Holds architected state, stats common across the SimpleCPU models.
    * Defines functions for checking for interrupts, setting up a fetch request, handling pre-execute setup, handling post-execute actions, and advancing the PC to the next instruction. These functions are also common across the SimpleCPU models.
    * Implements the ExecContext interface.
   The BaseSimpleCPU can not be run on its own. You must use one of the classes that inherits from BaseSimpleCPU, either AtomicSimpleCPU or TimingSimpleCPU.

  * ###### Atomic Simple CPU
  The AtomicSimpleCPU is the version of SimpleCPU that uses atomic memory accesses. It uses the latency estimates from the atomic accesses to estimate overall cache access time. The AtomicSimpleCPU is derived from BaseSimpleCPU, and implements functions to read and write memory, and also to tick, which defines what happens every CPU cycle. It defines the port that is used to hook up to memory, and connects the CPU to the cache.   
  Atomic accesses are faster than detailed access. They are used for fast forwarding and warming up caches and return an approximate time to complete the request without any resource contention or queuing delay. When a atomic access is sent the response is provided when the function returns.
  * ###### Timing Simple CPU
  The TimingSimpleCPU is the version of SimpleCPU that uses timing memory accesses*. It stalls on cache accesses and waits for the memory system to respond prior to proceeding. Like the AtomicSimpleCPU, the TimingSimpleCPU is also derived from BaseSimpleCPU, and implements the same set of functions. It defines the port that is used to hook up to memory, and connects the CPU to the cache. It also defines the necessary functions for handling the response from memory to the accesses sent out.
  Timing accesses are the most detailed access. They reflect our best effort for realistic timing and include the modeling of queuing delay and resource contention. Once a timing request is successfully sent at some point in the future the device that sent the request will either get the response or a NACK if the request could not be completed (more below).  
  The information above was taken from the [gem5 documentation site](http://gem5.org/SimpleCPU).

* #### 3a) Time of execution (Timing vs Minor)
 * TimingSimpleCPU: sim_seconds:  **0.000091** seconds
 * MinorCPU: sim_seconds: **0.000058** sim_seconds  
   Thus, MinorCPU is faster.

* #### 3b) Comparison of more parameters
 Examining the stats.txt files of both processes, we can conclude that the _"host"_ variables `host_inst_rate`, `host_op_rate` and  `host_tick_rate` are greater in TimingSimpleCPU, making the value of `host_seconds` greater in MinorCPU. However, `sim_seconds`, `sim_ticks` have larger values in TimingSimpleCPU. That happens because MinorCPU is more complicated, making the simulation process in the host more time consuming. Besides, as mentioned above, timing accesses are the most detailed access, including the modeling of queuing delay and resource contention. Finally, MinorCPU implements branch-prediction, while TimingSimpleCPU doesn't ([source](https://personal.utdallas.edu/~gxm112130/EE6304FA17/project2.pdf?fbclid=IwAR2ff3Kxnzb_Z9BI2gNhRZp6EFlOGktGyjFNGMMV9J8E57u8Ji0H2f9ZdSs), page 2).

* #### 3c) More tests with different frequency and memory type
  * ##### **Frequency**:
   * ###### 1GHz
    * **MinorCPU**:
    ```
    host_inst_rate: 154337
    host_op_rate: 182901
    host_seconds: 0.27
    sim_seconds: 0.000085
    system.cpu.cpi: 2.016168
    ```
    * **TimingSimpleCPU**:
    ```
    host_inst_rate: 469919
    host_op_rate: 55402
    host_seconds: 0.09
    sim_seconds: 0.000156
    ```
   * ###### 4GHz
    * **MinorCPU**:
    ```
    host_inst_rate: 138295
    host_op_rate: 16899
    host_seconds: 0.3
    sim_seconds: 0.000045
    system.cpu.cpi: 4.256399
    ```
    * **TimingSimpleCPU**:
    ```
    host_inst_rate: 320471
    host_op_rate: 377844
    host_seconds: 0.13
    sim_seconds: 0.000059
    ```
    * We can see that the CPI for the 4GHz drops almost two times. As the frequency increases by a factor of 4, the second simulation is expected to be two times faster. That is varified by the reduction in `sim_seconds`.
  * ##### **Memory type**:
   * ###### DDR3_2133_8x8
    * **MinorCPU**:
    ```
    host_inst_rate: 160378
    host_op_rate: 190030
    host_seconds: 0.26
    sim_seconds: 0.000057
    system.cpu.cpi: 2.709765
    ```
    * **TimingSimpleCPU**:
    ```
    host_inst_rate: 554813
    host_op_rate: 653789
    host_seconds: 0.08
    sim_seconds: 0.000090
    ```
   * ###### DDR4_2400_4x16
    * **MinorCPU**:
    ```
    host_inst_rate: 138414
    host_op_rate: 163933
    host_seconds: 0.3
    sim_seconds: 0.000058
    system.cpu.cpi: 2.771769
    ```
    * **TimingSimpleCPU**:
    ```
    host_inst_rate: 415361
    host_op_rate: 489760
    host_seconds: 0.1
    sim_seconds: 0.000092
    ```
    * The results may seem absurd at first sight. However, our guess is that it's due to fact that DDR4 has slightly higher [latency](https://www.wepc.com/reviews/ddr3-vs-ddr4/?fbclid=IwAR0PZtmJ4COhFNRdYGDID9vWobj0ok2zd-FpJ5TInGoMa3QjUYeur6wl92Y) when it comes to simple tasks (like our program!). Moreover, we have chosen the highest frequency DDR3 available, comparing it with a relatively low speed DDR4. In a scenario in which the frequency difference is higher, we assume that the DDR4 memory would outperform DDR3.
