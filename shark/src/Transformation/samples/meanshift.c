struct vec3 {
  double x;
  double y;
  double z;
};
#define N 40
#define EPSILON 0.00005

double L2(struct vec3 p1, struct vec3 p2) {
  double x = p1.x - p2.x;
  double y = p1.y - p2.y;
  double z = p1.z - p2.z;
  return x * x + y * y + z * z;
}

double epanechnikovKernel(double x) {
  if (x < 1.0) {
    return 1 - x;
  }
  return 0.0;
}

void shift_point(const struct vec3 point, const struct vec3 points[N],
                 struct vec3 *shifted_point) {
  for (int i = 0; i < N; i++) {
    const struct vec3 temp_point = points[i];
    double distance = L2(point, temp_point);
    double weight = epanechnikovKernel(distance);
    shifted_point->x += temp_point.x * weight;
    shifted_point->y += temp_point.y * weight;
    shifted_point->z += temp_point.z * weight;
  }
}

void meanshift(struct vec3 points[N], struct vec3 shifted_points[N]) {
  const double EPSILON_SQR = EPSILON * EPSILON;
  int stop_moving[N];
  for (int i = 0; i < N; i++) {
    stop_moving[i] = 0;
  }
  double max_shift_distance;
  struct vec3 point_new;
  do {
    max_shift_distance = 0;
    for (int i = 0; i < N; i++) {
      if (!stop_moving[i]) {
        shift_point(shifted_points[i], points, &point_new);
        double shift_distance_sqr = L2(point_new, shifted_points[i]);
        if (shift_distance_sqr > max_shift_distance) {
          max_shift_distance = shift_distance_sqr;
        }
        if (shift_distance_sqr <= EPSILON_SQR) {
          stop_moving[i] = 1;
        }
        shifted_points[i] = point_new;
      }
    }
  } while (max_shift_distance > EPSILON_SQR);
}
