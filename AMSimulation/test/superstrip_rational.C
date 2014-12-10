#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


// Default parameters: SF1.0
unsigned superstrip_rational(unsigned lay, float phi, float eta,
                             const float unit_scale=1.0) {
    unsigned h = 0;
    //lay = compressLayer(lay);  // transform lay

    static const float units_phi[6] = {0.00381, 0.00439, 0.00459, 0.00485, 0.00523, 0.00575};
    static const float units_eta[6] = {4.4, 4.4, 4.4, 4.4, 4.4, 4.4};
    float unit_phi = units_phi[0] * unit_scale;
    float unit_eta = units_eta[0] * 1.0;  // unaffected by 'unit_scale'
    int n_phi = floor(M_PI*2. / unit_phi + 0.5);
    int n_eta = floor(2.2*2.  / unit_eta + 0.5);
    assert(n_phi > 0 && n_eta > 0);

    if (lay < 16) {
        unit_phi = units_phi[lay] * unit_scale;
        unit_eta = units_eta[lay] * 1.0;  // unaffected by 'unit_scale'

        phi += M_PI;  // -M_PI is the lowest phi value
        eta += 2.2;   // -2.2  is the lowest eta value

        int i_phi = floor(phi / unit_phi);
        int i_eta = floor(eta / unit_eta);

        i_phi = (i_phi < 0) ? 0 : (i_phi >= n_phi) ? (n_phi - 1) : i_phi;  // proper range
        i_eta = (i_eta < 0) ? 0 : (i_eta >= n_eta) ? (n_eta - 1) : i_eta;  // proper range

        h = (lay * n_eta * n_phi) + i_eta * n_phi + i_phi;

    } else if (lay < 18) {
        h = lay * n_eta * n_phi;

    } else {
        h = 18 * n_eta * n_phi;

    }

    return h;
}
