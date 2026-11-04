#ifndef B5E655EC_CF68_4D36_894C_486C0A5BF5FB
#define B5E655EC_CF68_4D36_894C_486C0A5BF5FB
#include <algorithm>           
#include <array>               
#include <atomic>              
#include <chrono>
#include <condition_variable>  
#include <climits>             
#include <cmath>               
#include <csignal>
#include <cstdarg>             
#include <cstddef>             
#include <cstdio>              
#include <cstdlib>
#include <cstring>             
#include <ctime>               
#include <deque>               
#include <exception>
#include <forward_list>        
#include <fstream>             
#include <functional>          
#include <future>
#include <iomanip>             
#include <iostream>            
#include <iterator>            
#include <list>
#include <map>                 
#include <memory>              
#include <mutex>               
#include <ostream>
#include <queue>               
#include <random>              
#include <set>                 
#include <stdexcept>
#include <string>              
#include <sstream>             
#include <thread>              
#include <tuple>          
#include <utility>             
#include <vector>


// CVICENI///

std::pair<int, std::string> foo(int a, int b) {
    int summ = a + b;
    if (a + b > 10) {
        return {summ, "Success"};
    } 

    return {summ, "Error"};
}



#endif /* B5E655EC_CF68_4D36_894C_486C0A5BF5FB */
