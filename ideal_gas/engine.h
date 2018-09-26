/// Particle of the gas
struct body_t {
public:
    double x,  y,  z;
    double vx, vy, vz;
};

/// Container size
struct field_t {
public:
    double xmin, ymin, zmin;
    double xmax, ymax, zmax;

    double vxmin;
};

/// Experiment shell
struct shell_t {
public:
    /// Particles
    body_t *body_arr = nullptr;
    size_t n_body = 0;
    double body_r = 0;

    /// Container
    field_t field;

    /// Main timer
    double timer = 0;

    /// meas_pressure tmp values
    double timer_pr_start = 0;
    double sumary_hit_impulse = 0;

    /// Random init
    void init_bodies(size_t n_body_, double vmax);

    /// Physics
    void process_step(double dt);
    void process_coord(double dt);
    void process_collisions();

    /// Meas. V of vessel
    double meas_capacity();

    /// Meas. S of vessel
    double meas_square();

    /// Meas sqrt(<v^2>)
    double meas_rmsv();

    /// Meas <v>
    double meas_ev();

    /// p(v belongs to [v1, v2))
    void meas_npartv();
    size_t* get_v_distr(size_t *arr, double v_max, int n_seg);

    /// Measure pressure
    void begin_meas_pressure();
    double meas_pressure();
};
