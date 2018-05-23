double f(double x) { return x * x; }

double f_(double x) { return 2 * x; }

double find_root(const double x_initial) {
  const double error_bound = 0.005;
  const int iteration_bound = 100;

  double error = 1.0;
  double x = x_initial;
  int iterations = 0;
  do {
    double x_next = x - f(x) / f_(x);
    error = x - x_next;
    x = x_next;
  } while (error > error_bound && iterations < iteration_bound);

  return x;
}
