// __BEGIN_LICENSE__
// Copyright (C) 2006-2009 United States Government as represented by
// the Administrator of the National Aeronautics and Space Administration.
// All Rights Reserved.
// __END_LICENSE__


// TestPerPixelAccessorView.h
#include <cxxtest/TestSuite.h>

#include <vw/Image/PerPixelAccessorViews.h>
#include <vw/Image/ImageView.h>
#include <vw/Core/Functors.h>

using namespace vw;

class TestPerPixelViews : public CxxTest::TestSuite
{
public:

  template <template<class> class TraitT, class T>
  static bool bool_trait( T const& arg ) {
    return TraitT<T>::value;
  }

  template <class T1, class T2>
  static bool has_pixel_type( T2 ) {
    return boost::is_same<T1,typename T2::pixel_type>::value;
  }

  // Pixel accessor function
  struct AccessorNegationFunctor : public ReturnFixedType<float> {
    BBox2i work_area() const { return BBox2i(0,0,1,1); }

    template <class PixelAccessorT>
    typename boost::remove_reference<typename PixelAccessorT::result_type>::type
    operator()(PixelAccessorT acc) const {
      return -(*acc);
    }
  };

  void test_unary_per_pixel_accessor_view_function() {
    ImageView<float> im(2,2); im(0,0)=1; im(1,0)=2; im(0,1)=3; im(1,1)=4;
    UnaryPerPixelAccessorView<ImageView<float>, AccessorNegationFunctor> ppv( im, AccessorNegationFunctor() );
    TS_ASSERT_EQUALS( ppv.cols(), 2 );
    TS_ASSERT_EQUALS( ppv.rows(), 2 );
    TS_ASSERT_EQUALS( ppv.planes(), 1 );

    // Test individual pixel access
    TS_ASSERT_EQUALS( ppv(0,0), -1 );
    TS_ASSERT_EQUALS( ppv(1,0), -2 );
    TS_ASSERT_EQUALS( ppv(0,1), -3 );
    TS_ASSERT_EQUALS( ppv(1,1), -4 );

    // Test full rasterizaion
    ImageView<float> im2 = ppv;
    TS_ASSERT_EQUALS( im2.cols(), 2 );
    TS_ASSERT_EQUALS( im2.rows(), 2 );
    TS_ASSERT_EQUALS( im2.planes(), 1 );
    TS_ASSERT_EQUALS( im2(0,0), ppv(0,0) );
    TS_ASSERT_EQUALS( im2(1,0), ppv(1,0) );
    TS_ASSERT_EQUALS( im2(0,1), ppv(0,1) );
    TS_ASSERT_EQUALS( im2(1,1), ppv(1,1) );

    // Test partial rasterization
    ImageView<float> im3(1,2);
    TS_ASSERT_THROWS_NOTHING( ppv.rasterize( im3, BBox2i(1,0,1,2) ) );
    TS_ASSERT_EQUALS( im3(0,0), ppv(1,0) );
    TS_ASSERT_EQUALS( im3(0,1), ppv(1,1) );
    ImageView<float> im4(2,1);
    TS_ASSERT_THROWS_NOTHING( ppv.rasterize( im4, BBox2i(0,1,2,1) ) );
    TS_ASSERT_EQUALS( im4(0,0), ppv(0,1) );
    TS_ASSERT_EQUALS( im4(1,0), ppv(1,1) );

    // Test the accessor / generic rasterization
    ImageView<float> im5(2,2);
    vw::rasterize( ppv, im5, BBox2i(0,0,2,2) );
    TS_ASSERT_EQUALS( im5(0,0), ppv(0,0) );
    TS_ASSERT_EQUALS( im5(1,0), ppv(1,0) );
    TS_ASSERT_EQUALS( im5(0,1), ppv(0,1) );
    TS_ASSERT_EQUALS( im5(1,1), ppv(1,1) );

    // Test the iterator
    ImageView<float>::iterator im2i = im2.begin();
    UnaryPerPixelAccessorView<ImageView<float>, AccessorNegationFunctor>::iterator ppvi = ppv.begin();
    for( int i=0; i<4; ++i ) {
      TS_ASSERT_DIFFERS( ppvi, ppv.end() );
      TS_ASSERT_EQUALS( *im2i, *ppvi );
      TS_ASSERT_THROWS_NOTHING( ++ppvi );
      ++im2i;
    }
    TS_ASSERT_EQUALS( ppvi, ppv.end() );

    // Test the types
    TS_ASSERT( has_pixel_type<float>( ppv ) );
    TS_ASSERT( !bool_trait<IsMultiplyAccessible>(ppv) );
    TS_ASSERT( bool_trait<IsImageView>(ppv) );
  }

};
