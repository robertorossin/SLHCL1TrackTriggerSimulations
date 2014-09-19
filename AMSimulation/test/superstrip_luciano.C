unsigned luciano(unsigned lay, float z, float phi,
                                               const float unit_z=640., const float unit_phi=M_PI/1600.) {
    unsigned h = 0;
    //lay = compressLayer(lay);  // transform lay

    const unsigned n_z = 320*2. / unit_z + 1e-3;
    const unsigned n_phi = M_PI*2. / unit_phi + 1e-3;

    if (lay < 16) {
        z += 320.;    // -320. is the lowest z
        phi += M_PI;  // -M_PI is the lowest phi value

        z = (z < 0.) ? 0. : (z >= 320.*2. ? 320.*2. - 1e-3 : z);
        phi = (phi < 0.) ? 0. : (phi >= M_PI*2. ? M_PI*2. - 1e-3 : phi);

        //std::cout << n_z << " " << n_phi <<  " " << unsigned(z / unit_z * n_phi) << " " << unsigned(phi / unit_phi) << std::endl;

        h = (lay * n_z * n_phi) + unsigned(z / unit_z) * n_phi + unsigned(phi / unit_phi);

    } else if (lay < 18) {
        h = lay * n_z * n_phi;

    } else {
        h = 18 * n_z * n_phi;

    }

    return h;
}
