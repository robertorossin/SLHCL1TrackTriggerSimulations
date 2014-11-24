unsigned superstrip_luciano(unsigned lay, float phi, float eta,
             const float unit_phi=M_PI/1600., const float unit_eta=4.4) {  // default: 400x0

    unsigned h = 0;
    //lay = compressLayer(lay);  // transform lay

    const int n_phi = floor(M_PI*2. / unit_phi + 0.5);
    const int n_eta = floor(2.2*2.  / unit_eta + 0.5);
    assert(n_phi > 0 && n_eta > 0);

    if (lay < 16) {
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

