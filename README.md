# DeformationFieldTransform / DisplacementFieldTransform

## General info

```C++
template<typename TParametersValueType, unsigned int NDimensions>
class itk::DisplacementFieldTransform< TParametersValueType, NDimensions >
```

This is an example that tests the DeformationFieldTransform or DisplacementFieldTransform (the name depending on the ITK version) [ITK] function.

It provides local/dense/high-dimensionality transformation via a displacement field.

The displacement field stores vectors of displacements, with dimension NDimensions. 
Transformation is performed at a given point by adding the displacement at that point to the input point.

T(x, p), x is the position, p is the local parameter at position x. For a 2D example:

x = (x0, x1), p = (p0, p1)

then T(x, p) is defined as:

T(x, p) = (T0, T1) = (x0+p0, x1+p1)

During transformation, out-of-bounds input points are returned with zero displacement.

The displacement field is defined using an itkImage, and must be set before use by the user, using SetDisplacementField. The image has the same dimensionality as the input and output spaces, defined by template parameter NDimensions, and is an image of vectors of type OutputVectorType, with dimensionality NDimensions as well.

An interpolator of type VectorInterpolateImageFunction is used with the displacement field image. By default, VectorLinearInterpolateImageFunction is used, and the user can override using SetInterpolator.

The displacement field data is stored using the common OptimizerParameters type in conjunction with the ImageVectorOptimizerParametersHelper class. This allows access of the displacement field image as if it were an itkArray, allowing transparent use with other classes.

Warning
    The SetParameters method will copy the passed parameters, which can be costly since displacement fields are dense and thus potentially very large.

The UpdateTransformParameters method simply adds the provided update array, applying the usual optional scaling factor. Derived classes may provide different behavior.

Because this is a local transform, methods that have a version that takes a point must be used, such as TransformVector, TransformCovariantVector, and TransformDiffusionTensor. Also, ComputeJacobianWithRespectToParameters simply returns an identity matrix (see method documentation), and ComputeJacobianWithRespectToPosition should be used.


## Related links
* https://itk.org/Wiki/ITK/Examples/Registration/DeformationFieldTransform
* https://itk.org/Doxygen/html/classitk_1_1DisplacementFieldTransform.html


## Build

Run the following to build and run:

```
cd DeformationFieldTransform/
cmake .
make
./DeformationFieldTransform
```

This will create the files

* deformation.mhd  
* deformation.raw  
* fixed.png  
* moving.png  
* output.png


[ITK]: <https://itk.org/>