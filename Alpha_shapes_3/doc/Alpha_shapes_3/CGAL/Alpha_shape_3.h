
namespace CGAL {

/*!
\ingroup PkgAlphaShapes3

The class `Alpha_shape_3` represents the family of 
alpha shapes of points in the 3D space for <I>all</I> real 
\f$ \alpha\f$. It maintains an underlying triangulation 
of the class `Dt`. Each k-dimensional face of Dt is associated with an 
interval that specifies for which values of alpha the face belongs to the alpha shape. 
The second template parameter `ExactAlphaComparisonTag` is a tag that, when set to 
`CGAL::Tag_true`, triggers exact comparisons between alpha values. This is useful 
when the Delaunay triangulation is instantiated with an exact predicates inexact constructions 
kernel. By default the `ExactAlphaComparisonTag` is set to `CGAL::Tag_false` as it induces a small 
overhead. Note that since such a strategy does not make sense if used together with a traits class with exact constructions, 
the tag `ExactAlphaComparisonTag` is not taken into account if `Dt::Geom_traits::FT` is not a floating point number type. 

Note that this class is at the same time used for <I>basic</I> and 
for <I>weighted</I> Alpha Shapes. 

### Inherits From ###

Inherits from `Dt`.

This class is the underlying triangulation class. 

The modifying functions `insert` and `remove` will overwrite 
the inherited functions. At the moment, only the static version is implemented. 

### I/O ###

The I/O operators are defined for `iostream`, and for 
the window stream provided by \cgal. The format for the iostream 
is an internal format. 

### Implementation ###

In `GENERAL` mode, the alpha intervals of each triangulation 
face is computed and stored at initialization time. 
In `REGULARIZED` mode, the alpha shape intervals of edges 
are not stored nor computed at initialization. 
Edges are simply classified on the fly upon request. 
This allows to have much faster building of alpha shapes in 
`REGULARIZED` mode. 

Function `Alpha_shape_3::alpha_find()` uses linear search, while 
`Alpha_shape_3::alpha_lower_bound()` and `Alpha_shape_3::alpha_upper_bound()` 
use binary search. 
`Alpha_shape_3::number_of_solid_components()` performs a graph traversal and takes time 
linear in the number of cells of the underlying triangulation. 
`Alpha_shape_3::find_optimal_alpha()` uses binary search and takes time 
\f$ O(n \log n)\f$, where \f$ n\f$ is the number of points. 

*/
template< typename Dt, typename ExactAlphaComparisonTag >
class Alpha_shape_3 : public Dt {
public:

/// \name Types 
/// @{

/*! 
the alpha shape traits type. 
It has to derive from a triangulation traits class. For example `Dt::Point` is a point class.
*/ 
typedef Hidden_type Gt; 

/*! 
the number type of alpha values. 

In case `ExactAlphaComparisonTag` is `CGAL::Tag_false`, it is Gt::FT. 

In case `ExactAlphaComparisonTag` is `CGAL::Tag_true`, it is a number type 
allowing filtered exact comparisons (that is, interval arithmetic is first used before 
resorting to exact arithmetic). Access to the interval containing the exact value is provided through the function 
`FT::Approximate_nt approx() const` where `FT::Approximate_nt` is `Interval_nt<Protected>` 
with `Protected=true`. Access to the exact value is provided through the function 
`FT::Exact_nt exact() const` where `FT::Exact_nt` depends on the configuration of %CGAL 
(it is `Gmpq` if `gmp` is available and `Quotient<CGAL::MP_Float>` otherwise). 
It must be noted that an object of type `FT` is valid as long as the alpha shapes class that creates 
it is valid and has not been modified. 
For convenience, classical comparison operators are provided for the type `FT`. 

*/ 
typedef Hidden_type FT; 

/*! 
The size type. 
*/ 
typedef Hidden_type size_type; 

/*! 
A bidirectional and non-mutable iterator that allow to traverse 
the increasing sequence of different alpha values. 
\pre Its `value_type` is `FT`. 
*/ 
typedef Hidden_type Alpha_iterator; 

/*! 
In GENERAL mode,  
In REGULARIZED mode,  
*/ 
enum Mode {GENERAL, /**< the alpha complex can have singular faces, 
                       i.e., faces of dimension \f$ k\f$, for \f$ k=(0,1,2)\f$ 
                       that are not subfaces of a \f$ k+1\f$ face of the complex.*/
           REGULARIZED /**< the complex is regularized, that is 
                          singular faces are dropped and the alpha complex 
                          includes only a subset of the tetrahedral cells 
                          of the triangulation and the subfaces of those cells. */
}; 

/*! 
Enum to classify the faces of the underlying 
triangulation with respect to the alpha shape. 

In GENERAL mode, for \f$ k=(0,1,2)\f$, 
each k-dimensional simplex of the triangulation 
can be classified as EXTERIOR, SINGULAR, REGULAR 
or INTERIOR. 
In GENERAL mode a \f$ k\f$ simplex is REGULAR if it is on the boundary 
f the alpha complex and belongs to a \f$ k+1\f$ simplex in this complex 
and it is SINGULAR if it is a boundary simplex that is not included in a \f$ k+1\f$ simplex of the complex. 

In REGULARIZED mode, for \f$ k=(0,1,2)\f$ 
each k-dimensional simplex of the triangulation 
can be classified as EXTERIOR, REGULAR or INTERIOR, i.e. 
there is no singular faces. 
A \f$ k\f$ simplex is REGULAR if it is on the boundary of alpha complex 
and belongs to a tetrahedral cell of the complex. 
*/ 
enum Classification_type {EXTERIOR, SINGULAR, REGULAR, INTERIOR}; 

/// @} 

/// \name Creation 
/// @{

/*! 
Introduces an empty alpha shape, sets the current alpha value to `alpha` and the mode to `m`. 
*/ 
Alpha_shape_3(FT alpha = 0, 
Mode m = REGULARIZED); 

/*! 
Builds an alpha shape of mode `m` 
from the triangulation `dt`. 
\attention This operation destroys the triangulation `dt`. 
*/ 
Alpha_shape_3(Dt& dt, 
FT alpha = 0, 
Mode m = REGULARIZED); 

/*! 
Builds an alpha shape of mode `m` for the points in the range 
`[first,last)` and  set the current alpha value to `alpha`. 
\tparam InputIterator must be an input iterator with value type `Point` (the point type of the underlying triangulation.) 
*/ 
template < class InputIterator > 
Alpha_shape_3( 
InputIterator first, 
InputIterator last, 
const FT& alpha = 0, 
Mode m = REGULARIZED); 

/// @} 

/// \name Modifiers 
/// @{

/*! 
Initialize the alpha shape data structure
for points in the range `[first,last)`. 
Returns the number of data points inserted in the underlying 
triangulation. 

If the function is applied to an non-empty alpha shape data structure, it is cleared 
before initialization. 
\tparam InputIterator must be an input iterator with value type `Point`. 
*/ 
template < class InputIterator > 
std::ptrdiff_t make_alpha_shape( 
InputIterator first, 
InputIterator last); 

/*! 
Clears the structure. 
*/ 
void 
clear(); 

/*! 
Sets the \f$ \alpha\f$-value to `alpha`. 
Returns the previous \f$ \alpha\f$-value. 
\pre `alpha` \f$ \geq0\f$. 
*/ 
FT 
set_alpha(const FT& alpha); 

/*! 
Sets the mode of the alpha shape to `GENERAL` or `REGULARIZED`. 
Returns the previous mode. 
Changing the mode of an alpha shape 
entails a partial re-computation of the data structure. 
*/ 
Mode 
set_mode(Mode m = REGULARIZED ); 

/// @} 

/// \name Query Functions 
/// @{

/*! 
Returns whether the alpha shape is general or regularized. 
*/ 
Mode 
get_mode(void) const; 

/*! 
Returns the current \f$ \alpha\f$-value. 
*/ 
const FT& 
get_alpha(void) const; 

/*! 
Returns the `n`-th `alpha`-value, sorted in an increasing order. 
\pre `n` < number of alphas. 
*/ 
const FT& get_nth_alpha(int n) const; 

/*! 
Returns the number of different alpha-values. 
*/ 
size_type number_of_alphas() const; 

/*! 
Locates a point `p` in the underlying triangulation and Classifies the 
associated k-face with respect to `alpha`. 
*/ 
Classification_type 
classify(const Point& p, 
const FT& alpha = get_alpha()) const; 

/*! 
Classifies the cell `f` of the underlying triangulation with 
respect 
to `alpha`. 
*/ 
Classification_type 
classify(Cell_handle f, const FT& alpha = get_alpha()) const; 

/*! 
Classifies the facet `f` of the underlying triangulation with 
respect to `alpha`. 
*/ 
Classification_type 
classify(Facet f, const FT& alpha = get_alpha()) const; 

/*! 
Classifies the facet of the cell `f` opposite to the vertex with index 
`i` 
of the underlying triangulation with respect to `alpha`. 
*/ 
Classification_type 
classify(Cell_handle f, int i, const FT& alpha = get_alpha()) const; 

/*! 
Classifies the edge `e` with respect to `alpha` . 
*/ 
Classification_type 
classify(const Edge& e, const FT& alpha = get_alpha()) const; 

/*! 
Classifies the vertex `v` of the underlying triangulation with respect to `alpha`. 
*/ 
Classification_type 
classify(Vertex_handle v, const FT& alpha = get_alpha()) const; 

/*! 
Write the cells which are of type `type` for 
the alpha value `alpha` to the sequence 
pointed to by the output iterator `it`. Returns past the end 
of the output sequence. 
*/ 
template<class OutputIterator> 
OutputIterator get_alpha_shape_cells(OutputIterator it, 
Classification_type type, 
const FT& alpha = get_alpha()); 

/*! 
Write the facets which are of type `type` for 
the alpha value `alpha` to the sequence 
pointed to by the output iterator `it`. Returns past the end 
of the output sequence. 
*/ 
template<class OutputIterator> 
OutputIterator get_alpha_shape_facets(OutputIterator it, 
Classification_type type, 
const FT& alpha= get_alpha()); 

/*! 
Write the edges which are of type `type` for 
the alpha value `alpha` to the sequence 
pointed to by the output iterator `it`. Returns past the end 
of the output sequence. 
*/ 
template<class OutputIterator> 
OutputIterator get_alpha_shape_edges(OutputIterator it, 
Classification_type type, 
const FT& alpha = get_alpha()); 

/*! 
Write the vertices which are of type `type` for 
the alpha value `alpha` to the sequence 
pointed to by the output iterator `it`. Returns past the end 
of the output sequence. 
*/ 
template<class OutputIterator> 
OutputIterator get_alpha_shape_vertices(OutputIterator it, 
Classification_type type, 
const FT& alpha); 

/*! 
Output all the faces of the triangulation 
in increasing order of the alpha value for which they appear 
in the alpha complex. In case of equal alpha value 
lower dimensional faces are output first. 

\tparam OutputIterator must be on output iterator with value type `Object`.
*/ 
template<class OutputIterator> 
OutputIterator filtration(OutputIterator it); 

/// @} 

/// \name Traversal of the alpha-Values 
/// @{

/*! 
Returns an iterator that allows to traverse the 
sorted sequence of \f$ \alpha\f$-values of the family of alpha shapes. 
*/ 
Alpha_iterator alpha_begin() const; 

/*! 
Returns the corresponding past-the-end iterator. 
*/ 
Alpha_iterator alpha_end() const; 

/*! 
Returns an iterator pointing to an element with \f$ \alpha\f$-value 
`alpha`, or the corresponding past-the-end iterator if such 
an element is not found. 
*/ 
Alpha_iterator alpha_find(const FT& alpha) const; 

/*! 
Returns an iterator pointing to the first element with 
\f$ \alpha\f$-value not less than `alpha`. 
*/ 
Alpha_iterator alpha_lower_bound(const FT& alpha) const; 

/*! 
Returns an iterator pointing to the first element with \f$ \alpha\f$-value 
greater than `alpha`. 
*/ 
Alpha_iterator alpha_upper_bound(const FT& alpha) const; 

/// @} 

/// \name Operations 
/// @{

/*! 
Returns the number of solid components of the alpha shape, that is, the number of 
components of its regularized version. 
*/ 
size_type number_of_solid_components(const FT& alpha = get_alpha()) const; 

/*! 
Returns an iterator pointing to smallest \f$ \alpha\f$ value 
such that the alpha shape satisfies the following two properties: 

all data points are either on the boundary or in the interior 
of the regularized version of the alpha shape. 

The number of solid component of the alpha shape is equal to or 
smaller than `nb_components`. 
*/ 
Alpha_iterator find_optimal_alpha(size_type nb_components) const; 


/// @}

}; /* end Alpha_shape_3 */

/*! 
Inserts the alpha shape `A` for the current alpha value into the stream `os`. 

Defined in \ref CGAL/IO/io.h

\pre The insert operator must be defined for `Point`. 
\relates Alpha_shape_3 
*/ 
  ostream& operator<<(std::ostream& os, 
const Alpha_shape_3<Dt,ExactAlphaComparisonTag>& A); 

/*! 
Inserts the alpha shape `A` for the current alpha value into the Geomview stream `W`. 
\pre The insert operator must be defined for `GT::Point` and `GT::Triangle`. 

Defined in \ref CGAL/IO/Geomview_stream.h and \ref CGAL/IO/alpha_shape_geomview_ostream_3.h

\relates Alpha_shape_3 
*/ 
Geomview_stream& operator<<(Geomview_stream& W, 
const Alpha_shape_3<Dt,ExactAlphaComparisonTag>& A); 

} /* end namespace CGAL */
