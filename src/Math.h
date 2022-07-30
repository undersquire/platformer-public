#pragma once

template <typename T> struct Vector2 {
  T x, y;

  Vector2() {}

  Vector2(T x, T y) : x(x), y(y) {}

  Vector2<T> operator+(T value) const {
    return {this->x + value, this->y + value};
  }

  Vector2<T> &operator+=(T value) {
    this->x += value;
    this->y += value;

    return *this;
  }

  Vector2<T> operator+(const Vector2<T> &other) const {
    return {this->x + other.x, this->y + other.y};
  }

  Vector2<T> &operator+=(const Vector2<T> &other) {
    this->x += other.x;
    this->y += other.y;

    return *this;
  }

  Vector2<T> operator-(T value) const {
    return {this->x - value, this->y - value};
  }

  Vector2<T> &operator-=(T value) {
    this->x -= value;
    this->y -= value;

    return *this;
  }

  Vector2<T> operator-(const Vector2<T> &other) const {
    return {this->x - other.x, this->y - other.y};
  }

  Vector2<T> &operator-=(const Vector2<T> &other) {
    this->x -= other.x;
    this->y -= other.y;

    return *this;
  }

  Vector2<T> operator*(T value) const {
    return {this->x * value, this->y * value};
  }

  Vector2<T> &operator*=(T value) {
    this->x *= value;
    this->y *= value;

    return *this;
  }

  Vector2<T> operator*(const Vector2<T> &other) const {
    return {this->x * other.x, this->y * other.y};
  }

  Vector2<T> &operator*=(const Vector2<T> &other) {
    this->x *= other.x;
    this->y *= other.y;

    return *this;
  }

  Vector2<T> operator/(T value) const {
    return {this->x / value, this->y / value};
  }

  Vector2<T> &operator/=(T value) {
    this->x /= value;
    this->y /= value;

    return *this;
  }

  Vector2<T> operator/(const Vector2<T> &other) const {
    return {this->x / other.x, this->y / other.y};
  }

  Vector2<T> &operator/=(const Vector2<T> &other) {
    this->x /= other.x;
    this->y /= other.y;

    return *this;
  }

  bool operator==(const Vector2<T> &other) {
    return this->x == other.x && this->y == other.y;
  }
};

template <class T>
inline Vector2<T> operator+(const T &value, const Vector2<T> &vec) {
  return {value + vec.x, value + vec.y};
}

template <class T>
inline Vector2<T> operator-(const T &value, const Vector2<T> &vec) {
  return {value - vec.x, value - vec.y};
}

template <class T>
inline Vector2<T> operator*(const T &value, const Vector2<T> &vec) {
  return {value * vec.x, value * vec.y};
}

template <class T>
inline Vector2<T> operator/(const T &value, const Vector2<T> &vec) {
  return {value / vec.x, value / vec.y};
}

struct Rectangle {
  Vector2<float> pos, size;

  bool Intersects(const Vector2<float> &) const noexcept;
  bool Intersects(const Rectangle &) const noexcept;
  bool Intersects(const Vector2<float> &, const Vector2<float> &,
                  Vector2<float> &, Vector2<float> &, float &) const noexcept;
  bool Intersects(const Rectangle &, const Vector2<float> &, const float,
                  Vector2<float> &, Vector2<float> &, float &) const noexcept;
  bool Intersects(const Vector2<float> &, const Rectangle &,
                  const Vector2<float> &, const float, Vector2<float> &,
                  Vector2<float> &, float &) const noexcept;
};

struct DynamicRectangle : Rectangle {
  Vector2<float> velocity;
};
