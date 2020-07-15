
//
// ------------------------------------------
// benchmark comparison program for [mulp-si]
// ------------------------------------------
//
// dependencies :
//
//      google / benchmark
//      [https://github.com/google/benchmark]
//
//      nushoin / RTree
//      [https://github.com/nushoin/RTree]
//
// command :
//      g++-10 -std=c++2a compare.cpp -o test -lbenchmark -lpthread
//

#include "mulpsi_alg/mulpsi.cpp"
#include "RTree/RTree.h"
#include "rect_gen.cpp"

#include <benchmark/benchmark.h>

#define SPC_RECT_NUM 100000

spacial::Rectangle rects[SPC_RECT_NUM], extrect(-300,300,0,600);
spacial::mulpsi<spacial::Rectangle *, int, 2> mulp;
RTree<spacial::Rectangle *, int, 2, float> rtre;

double tm = 0;

bool callback(spacial::Rectangle * id){
  return true;
}

void case_normal(benchmark::State & state){

    for(auto _ : state) {

        extrect.sx = 0;
        extrect.ex = 0;

        while(extrect.ex < SPC_RECT_POS_MAX){
            extrect.sx += 1;
            extrect.ex += 1;
            for(auto it:rects){
                if(extrect.ex >= it.sx and extrect.ey >= it.sy and extrect.sx <= it.ex and extrect.sy <= it.ey) callback(&it);
            }
        }

        //std::cout<<"check finished : case_normal"<<std::endl;

    }
}

void case_mulp(benchmark::State & state){

    mulp.init({extrect.sx, extrect.sy, extrect.ex, extrect.ey});

    for(auto &it:rects)
        mulp.insert(&it, {it.sx, it.sy, it.ex, it.ey});

    for(auto _ : state) {

        extrect.sx = 0;
        extrect.ex = 0;
        mulp.init({extrect.sx, extrect.sy, extrect.ex, extrect.ey});

        while(extrect.ex < SPC_RECT_POS_MAX){
            extrect.sx += 1;
            extrect.ex += 1;
            mulp.slide_pointer({extrect.sx, extrect.sy, extrect.ex, extrect.ey});
        }

        //std::cout<<"check finished : case_mulp"<<std::endl;

    }
}

void case_rtre(benchmark::State & state){
    for(auto &it:rects){
        const int min[2]={it.sx, it.sy}, max[2]={it.ex, it.ey};
        rtre.Insert(min, max, &it);
    }

    for(auto _ : state) {

        extrect.sx = 0;
        extrect.ex = 0;

        while(extrect.ex < SPC_RECT_POS_MAX){
            extrect.sx += 1;
            extrect.ex += 1;
            const int smin[2]={extrect.sx, extrect.sy};
            const int smax[2]={extrect.ex, extrect.ey};
            rtre.Search(smin, smax, callback);
        }

        //std::cout<<"check finished : case_rtre"<<std::endl;
    }
    
}

BENCHMARK(case_normal);
BENCHMARK(case_rtre);
BENCHMARK(case_mulp);

BENCHMARK_MAIN();
