
# Multipointer spacial index extraction algorithm (Mulpsix -> Mulp6)

**Mulp6** is an algorithm offers the best performance of **spacial index algorithm** in specific cases.

### performance

*B* : Best-performance <br>
*W* : Worst-performance <br>
*A* : Average-performance <br>

#### time complexity

**N** : the number of indexes

 - index insertion <br>
  *B*/*W*/*A* : **O(logN)** <br>

 - index deletion <br>
  *W*/*A* : **O(N)** <br>
  *B* : **O(1)** <br>
  
 - range extraction
   - primary process (on newly defined range) <br>
    *W*/*A* : **O(N)** <br>
    *B* : **O(1)** <br>
   - relocating, rescaling or primary process (on pre-defined range) <br>
   *B*/*W*/*A* : **O(1)** <br>


#### space complexity

**N** : the number of indexes <br>
**D** : the dimention <br>

 - **O(ND)**
 
 

