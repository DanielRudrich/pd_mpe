/*
 ==============================================================================
 Copyright (c) 2019 - Daniel Rudrich
 Released under GPLv3, see LICENSE.txt or <https://www.gnu.org/licenses/>.
 ==============================================================================
 */



#include "m_pd.h"
#include <math.h>
#include "RawMidiToMPE.h"

static t_class *mpe_class;

typedef struct _mpe
{
    t_object x_obj;
    t_float channelFilter;
    t_outlet *x_outlet;

    RawMidiToMPE* rawMidiToMPE;
} t_mpe;

static void *mpe_new (t_floatarg f)
{
    t_mpe *x = (t_mpe *) pd_new (mpe_class);
    x->channelFilter = f;
    x->x_outlet = outlet_new (&x->x_obj, &s_float);

    if (f == 0)
        x->rawMidiToMPE = new RawMidiToMPE (x->x_outlet);
    else
        x->rawMidiToMPE = new RawMidiToMPE (x->x_outlet, f - 1);

    // start listening to system midiin messages
    t_symbol *midiin = gensym ("#midiin");
    pd_bind (&x->x_obj.ob_pd, midiin);

    return x;
}

static void mpe_list (t_mpe *x, t_symbol *s, int ac, t_atom *av)
{
    const int byte = atom_getfloatarg (0, ac, av);
    const int portNumber = atom_getfloatarg (1, ac, av);
    x->rawMidiToMPE->processRawMidi (byte, portNumber);

}

static void mpe_free (t_mpe *x)
{
    // stop listening to system midiin messages
    t_symbol *midiin = gensym ("#midiin");
    pd_unbind (&x->x_obj.ob_pd, midiin);
}


extern "C" {

    void mpe_setup (void)
    {
        mpe_class = class_new (gensym ("mpe"), (t_newmethod) mpe_new,
                               (t_method) mpe_free, sizeof (t_mpe),
                               CLASS_NOINLET, A_DEFFLOAT, 0);
        class_sethelpsymbol(mpe_class, gensym("mpe"));
        class_addlist (mpe_class, mpe_list);
    }
}
