
#include <random>

#define SPC_RECT_SIZE_MIN   10
#define SPC_RECT_SIZE_MAX   2000
#define SPC_RECT_SIZE_RANGE (SPC_RECT_SIZE_MAX - SPC_RECT_SIZE_MIN + 1)

#define SPC_RECT_POS_MIN   0
#define SPC_RECT_POS_MAX   1000000
#define SPC_RECT_POS_RANGE (SPC_RECT_POS_MAX - SPC_RECT_POS_MIN + 1)

namespace spacial{

    std::random_device rnd;

    struct Rectangle{

        int sx, sy, ex, ey;
        Rectangle(){
            sx = SPC_RECT_POS_MIN + (rnd()%SPC_RECT_POS_RANGE) ;
            sy = SPC_RECT_POS_MIN + (rnd()%SPC_RECT_POS_RANGE) ;
            ex = sx + SPC_RECT_SIZE_MIN + (rnd()%SPC_RECT_SIZE_RANGE) ;
            ey = sy + SPC_RECT_SIZE_MIN + (rnd()%SPC_RECT_SIZE_RANGE) ;
        }

        Rectangle(int _sx, int _sy, int _ex, int _ey){
            sx = _sx; sy = _sy;
            ex = _ex; ey = _ey;
        }

        #ifdef Capr

        void draw(Capr::Cairo_cont cr, double r, double g, double b){
            cr->set_source_rgba(r, g, b, 0.1);
            cr->rectangle(sx, sy, ex-sx, ey-sy);
            cr->fill_preserve();

            cr->set_source_rgba(0.0, 0.0, 0.0, 1.0);
            cr->set_line_width(1.0);
            cr->stroke();
        }

        #endif
    };
}
