// __BEGIN_LICENSE__
// Copyright (C) 2006-2009 United States Government as represented by
// the Administrator of the National Aeronautics and Space Administration.
// All Rights Reserved.
// __END_LICENSE__


// TestCAHVORModel.h
#include <cxxtest/TestSuite.h>

#include <vw/Camera/CAHVORModel.h>
#include <vw/Math/Vector.h>

using namespace vw;
using namespace vw::camera;

class TestCAHVORModel : public CxxTest::TestSuite
{
public:

  void test_get_point_derivatives()
  {
    CAHVORModel cahvor;
    /*
    cahvor.C = Vector3(0,0,-10);
    cahvor.A = Vector3(0,0,1);
    cahvor.H = Vector3(1000,0,320);
    cahvor.V = Vector3(0,1000,240);
    cahvor.O = Vector3(0,0,1);
    cahvor.R = Vector3(0,-0.2,.2);
    */
    // A random camera model to avoid symmetries.
    cahvor.C = Vector3(76,-34,20);
    cahvor.A = normalize(Vector3(1,2,3));
    cahvor.H = Vector3(1300,123,456);
    cahvor.V = Vector3(345,900,157);
    cahvor.O = normalize(Vector3(2,3,4));
    cahvor.R = Vector3(1,-1,1);
    double u, v;
    Vector3 grad_u, grad_v;
    Matrix3x3 hess_u, hess_v;
    Vector3 P(0,0,0);
    cahvor.get_point_derivatives( P, u, v, grad_u, grad_v, hess_u, hess_v );
    Vector2 ref = cahvor.point_to_pixel( P );
    double delta = 0.001;
    Vector2 refgx = (cahvor.point_to_pixel( P+Vector3(delta,0,0) )-ref)/delta;
    Vector2 refgy = (cahvor.point_to_pixel( P+Vector3(0,delta,0) )-ref)/delta;
    Vector2 refgz = (cahvor.point_to_pixel( P+Vector3(0,0,delta) )-ref)/delta;
    Vector2 refhxx = ( ( cahvor.point_to_pixel( P+Vector3(2*delta,0,0) ) -
                         cahvor.point_to_pixel( P+Vector3(delta,0,0) ) ) -
                       ( cahvor.point_to_pixel( P+Vector3(delta,0,0) ) -
                         cahvor.point_to_pixel( P+Vector3(0,0,0) ) ) ) / (delta*delta);
    Vector2 refhxy = ( ( cahvor.point_to_pixel( P+Vector3(delta,delta,0) ) -
                         cahvor.point_to_pixel( P+Vector3(0,delta,0) ) ) -
                       ( cahvor.point_to_pixel( P+Vector3(delta,0,0) ) -
                         cahvor.point_to_pixel( P+Vector3(0,0,0) ) ) ) / (delta*delta);
    Vector2 refhxz = ( ( cahvor.point_to_pixel( P+Vector3(delta,0,delta) ) -
                         cahvor.point_to_pixel( P+Vector3(0,0,delta) ) ) -
                       ( cahvor.point_to_pixel( P+Vector3(delta,0,0) ) -
                         cahvor.point_to_pixel( P+Vector3(0,0,0) ) ) ) / (delta*delta);
    Vector2 refhyx = ( ( cahvor.point_to_pixel( P+Vector3(delta,delta,0) ) -
                         cahvor.point_to_pixel( P+Vector3(delta,0,0) ) ) -
                       ( cahvor.point_to_pixel( P+Vector3(0,delta,0) ) -
                         cahvor.point_to_pixel( P+Vector3(0,0,0) ) ) ) / (delta*delta);
    Vector2 refhyy = ( ( cahvor.point_to_pixel( P+Vector3(0,2*delta,0) ) -
                         cahvor.point_to_pixel( P+Vector3(0,delta,0) ) ) -
                       ( cahvor.point_to_pixel( P+Vector3(0,delta,0) ) -
                         cahvor.point_to_pixel( P+Vector3(0,0,0) ) ) ) / (delta*delta);
    Vector2 refhyz = ( ( cahvor.point_to_pixel( P+Vector3(0,delta,delta) ) -
                         cahvor.point_to_pixel( P+Vector3(0,0,delta) ) ) -
                       ( cahvor.point_to_pixel( P+Vector3(0,delta,0) ) -
                         cahvor.point_to_pixel( P+Vector3(0,0,0) ) ) ) / (delta*delta);
    Vector2 refhzx = ( ( cahvor.point_to_pixel( P+Vector3(delta,0,delta) ) -
                         cahvor.point_to_pixel( P+Vector3(delta,0,0) ) ) -
                       ( cahvor.point_to_pixel( P+Vector3(0,0,delta) ) -
                         cahvor.point_to_pixel( P+Vector3(0,0,0) ) ) ) / (delta*delta);
    Vector2 refhzy = ( ( cahvor.point_to_pixel( P+Vector3(0,delta,delta) ) -
                         cahvor.point_to_pixel( P+Vector3(0,delta,0) ) ) -
                       ( cahvor.point_to_pixel( P+Vector3(0,0,delta) ) -
                         cahvor.point_to_pixel( P+Vector3(0,0,0) ) ) ) / (delta*delta);
    Vector2 refhzz = ( ( cahvor.point_to_pixel( P+Vector3(0,0,2*delta) ) -
                         cahvor.point_to_pixel( P+Vector3(0,0,delta) ) ) -
                       ( cahvor.point_to_pixel( P+Vector3(0,0,delta) ) -
                         cahvor.point_to_pixel( P+Vector3(0,0,0) ) ) ) / (delta*delta);
    TS_ASSERT_DELTA( u, ref[0], 1e-5 );
    TS_ASSERT_DELTA( v, ref[1], 1e-5 );
    TS_ASSERT_DELTA( grad_u[0], refgx[0], 0.01 );
    TS_ASSERT_DELTA( grad_u[1], refgy[0], 0.01 );
    TS_ASSERT_DELTA( grad_u[2], refgz[0], 0.01 );
    TS_ASSERT_DELTA( grad_v[0], refgx[1], 0.01 );
    TS_ASSERT_DELTA( grad_v[1], refgy[1], 0.01 );
    TS_ASSERT_DELTA( grad_v[2], refgz[1], 0.01 );
    TS_ASSERT_DELTA( hess_u(0,0), refhxx[0], 0.01 );
    TS_ASSERT_DELTA( hess_u(1,0), refhyx[0], 0.01 );
    TS_ASSERT_DELTA( hess_u(2,0), refhzx[0], 0.01 );
    TS_ASSERT_DELTA( hess_u(0,1), refhxy[0], 0.01 );
    TS_ASSERT_DELTA( hess_u(1,1), refhyy[0], 0.01 );
    TS_ASSERT_DELTA( hess_u(2,1), refhzy[0], 0.01 );
    TS_ASSERT_DELTA( hess_u(0,2), refhxz[0], 0.01 );
    TS_ASSERT_DELTA( hess_u(1,2), refhyz[0], 0.01 );
    TS_ASSERT_DELTA( hess_u(2,2), refhzz[0], 0.01 );
    TS_ASSERT_DELTA( hess_v(0,0), refhxx[1], 0.01 );
    TS_ASSERT_DELTA( hess_v(1,0), refhyx[1], 0.01 );
    TS_ASSERT_DELTA( hess_v(2,0), refhzx[1], 0.01 );
    TS_ASSERT_DELTA( hess_v(0,1), refhxy[1], 0.01 );
    TS_ASSERT_DELTA( hess_v(1,1), refhyy[1], 0.01 );
    TS_ASSERT_DELTA( hess_v(2,1), refhzy[1], 0.01 );
    TS_ASSERT_DELTA( hess_v(0,2), refhxz[1], 0.01 );
    TS_ASSERT_DELTA( hess_v(1,2), refhyz[1], 0.01 );
    TS_ASSERT_DELTA( hess_v(2,2), refhzz[1], 0.01 );
    /*
    TS_TRACE(stringify("ref=")    + stringify(ref));
    TS_TRACE(stringify("u=")      + stringify(u));
    TS_TRACE(stringify("grad_u=") + stringify(grad_u));
    TS_TRACE(stringify("grad_v=") + stringify(grad_v));
    TS_TRACE(stringify("hess_u=") + stringify(hess_u));
    TS_TRACE(stringify("hess_v=") + stringify(hess_v));
    TS_TRACE(stringify("ddx=")    + stringify(refgx));
    TS_TRACE(stringify("ddy=")    + stringify(refgy));
    TS_TRACE(stringify("ddz=")    + stringify(refgz));
    TS_TRACE(stringify("d2dx2=")  + stringify(refhxx));
    TS_TRACE(stringify("d2dxdy=") + stringify(refhxy));
    TS_TRACE(stringify("d2dxdz=") + stringify(refhxz));
    TS_TRACE(stringify("d2dydx=") + stringify(refhyx));
    TS_TRACE(stringify("d2dy2=")  + stringify(refhyy));
    TS_TRACE(stringify("d2dydz=") + stringify(refhyz));
    TS_TRACE(stringify("d2dzdx=") + stringify(refhzx));
    TS_TRACE(stringify("d2dzdy=") + stringify(refhzy));
    TS_TRACE(stringify("d2dz2=")  + stringify(refhzz));
    */
  }

  void test_StringRead() {
    CAHVORModel c(TEST_SRCDIR"/cahvor.txt");

    Vector3 C = Vector3(76,-34,20);
    Vector3 A = normalize(Vector3(1,2,3));
    Vector3 H = Vector3(1300,123,456);
    Vector3 V = Vector3(345,900,157);
    Vector3 O = normalize(Vector3(2,3,4));
    Vector3 R = Vector3(1,-1,1);

    TS_ASSERT_DELTA( C(0), c.C(0), 1e-5 );
    TS_ASSERT_DELTA( C(1), c.C(1), 1e-5 );
    TS_ASSERT_DELTA( C(2), c.C(2), 1e-5 );
    TS_ASSERT_DELTA( A(0), c.A(0), 1e-5 );
    TS_ASSERT_DELTA( A(1), c.A(1), 1e-5 );
    TS_ASSERT_DELTA( A(2), c.A(2), 1e-5 );
    TS_ASSERT_DELTA( H(0), c.H(0), 1e-5 );
    TS_ASSERT_DELTA( H(1), c.H(1), 1e-5 );
    TS_ASSERT_DELTA( H(2), c.H(2), 1e-5 );
    TS_ASSERT_DELTA( V(0), c.V(0), 1e-5 );
    TS_ASSERT_DELTA( V(1), c.V(1), 1e-5 );
    TS_ASSERT_DELTA( V(2), c.V(2), 1e-5 );
    TS_ASSERT_DELTA( O(0), c.O(0), 1e-5 );
    TS_ASSERT_DELTA( O(1), c.O(1), 1e-5 );
    TS_ASSERT_DELTA( O(2), c.O(2), 1e-5 );
    TS_ASSERT_DELTA( R(0), c.R(0), 1e-5 );
    TS_ASSERT_DELTA( R(1), c.R(1), 1e-5 );
    TS_ASSERT_DELTA( R(2), c.R(2), 1e-5 );
  }

  void test_StringWriteRead() {
    CAHVORModel c;

    c.C = Vector3(76,-34,20);
    c.A = normalize(Vector3(1,2,3));
    c.H = Vector3(1300,123,456);
    c.V = Vector3(345,900,157);
    c.O = normalize(Vector3(2,3,4));
    c.R = Vector3(1,-1,1);
    c.write(TEST_SRCDIR"/cahvor2.txt");

    CAHVORModel c2(TEST_SRCDIR"/cahvor2.txt");

    TS_ASSERT_DELTA( c2.C(0), c.C(0), 1e-5 );
    TS_ASSERT_DELTA( c2.C(1), c.C(1), 1e-5 );
    TS_ASSERT_DELTA( c2.C(2), c.C(2), 1e-5 );
    TS_ASSERT_DELTA( c2.A(0), c.A(0), 1e-5 );
    TS_ASSERT_DELTA( c2.A(1), c.A(1), 1e-5 );
    TS_ASSERT_DELTA( c2.A(2), c.A(2), 1e-5 );
    TS_ASSERT_DELTA( c2.H(0), c.H(0), 1e-5 );
    TS_ASSERT_DELTA( c2.H(1), c.H(1), 1e-5 );
    TS_ASSERT_DELTA( c2.H(2), c.H(2), 1e-5 );
    TS_ASSERT_DELTA( c2.V(0), c.V(0), 1e-5 );
    TS_ASSERT_DELTA( c2.V(1), c.V(1), 1e-5 );
    TS_ASSERT_DELTA( c2.V(2), c.V(2), 1e-5 );
    TS_ASSERT_DELTA( c2.O(0), c.O(0), 1e-5 );
    TS_ASSERT_DELTA( c2.O(1), c.O(1), 1e-5 );
    TS_ASSERT_DELTA( c2.O(2), c.O(2), 1e-5 );
    TS_ASSERT_DELTA( c2.R(0), c.R(0), 1e-5 );
    TS_ASSERT_DELTA( c2.R(1), c.R(1), 1e-5 );
    TS_ASSERT_DELTA( c2.R(2), c.R(2), 1e-5 );
  }
};
