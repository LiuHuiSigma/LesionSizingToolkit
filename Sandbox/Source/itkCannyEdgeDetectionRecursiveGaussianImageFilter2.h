/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkCannyEdgeDetectionRecursiveGaussianImageFilter2.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkCannyEdgeDetectionRecursiveGaussianImageFilter_h
#define __itkCannyEdgeDetectionRecursiveGaussianImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkFixedArray.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkSmoothingRecursiveGaussianImageFilter.h"
#include "itkLaplacianRecursiveGaussianImageFilter.h"
#include "itkMultiplyImageFilter.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkMultiThreader.h"
#include "itkDerivativeOperator.h"
#include "itkSparseFieldLayer.h"
#include "itkObjectStore.h"


namespace itk
{


template <class TValueType>
class ListNode
{
public:
  TValueType m_Value;

  ListNode   *Next;
  ListNode   *Previous;
};


/** \class CannyEdgeDetectionRecursiveGaussianImageFilter
 *
 * This filter is an implementation of a Canny edge detector for scalar-valued
 * images.  Based on John Canny's paper "A Computational Approach to Edge 
 * Detection"(IEEE Transactions on Pattern Analysis and Machine Intelligence, 
 * Vol. PAMI-8, No.6, November 1986),  there are four major steps used in the 
 * edge-detection scheme:
 * (1) Smooth the input image with Gaussian filter.
 * (2) Calculate the second directional derivatives of the smoothed image. 
 * (3) Non-Maximum Suppression: the zero-crossings of 2nd derivative are found,
 *     and the sign of third derivative is used to find the correct extrema. 
 * (4) The hysteresis thresholding is applied to the gradient magnitude
 *      (multiplied with zero-crossings) of the smoothed image to find and 
 *      link edges.
 *
 * \par Inputs and Outputs
 * The input to this filter should be a scalar, real-valued Itk image of
 * arbitrary dimension.  The output should also be a scalar, real-value Itk
 * image of the same dimensionality.
 *
 * \par Parameters
 * There are four parameters for this filter that control the sub-filters used
 * by the algorithm.
 *
 * \par 
 * Sigma is used in the Gaussian smoothing of the input image.
 * See  itkSmoothingRecursiveGaussianImageFilter for information on these
 * parameters.
 *
 * \par
 * Threshold is the lowest allowed value in the output image.  Its data type is 
 * the same as the data type of the output image. Any values below the
 * Threshold level will be replaced with the OutsideValue parameter value, whose
 * default is zero.
 * 
 * \todo Edge-linking will be added when an itk connected component labeling
 * algorithm is available.
 *
 * \sa SmoothingRecursiveGaussianImageFilter
 * \sa ZeroCrossingImageFilter
 * \sa ThresholdImageFilter */
template<class TInputImage, class TOutputImage>
class ITK_EXPORT CannyEdgeDetectionRecursiveGaussianImageFilter
  : public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard "Self" & Superclass typedef.  */
  typedef CannyEdgeDetectionRecursiveGaussianImageFilter                 Self;
  typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;
   
  /** Image typedef support   */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;
      
  /** SmartPointer typedef support  */
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Define pixel types. */
  typedef typename TInputImage::PixelType   InputImagePixelType;
  typedef typename TOutputImage::PixelType  OutputImagePixelType;
  typedef typename TInputImage::IndexType   IndexType;

  /** The default boundary condition is used unless overridden 
   *in the Evaluate() method. */
  typedef ZeroFluxNeumannBoundaryCondition<OutputImageType>
  DefaultBoundaryConditionType;

  /** The type of data structure that is passed to this function object
   * to evaluate at a pixel that does not lie on a data set boundary.
   */
  typedef ConstNeighborhoodIterator<OutputImageType,
                                    DefaultBoundaryConditionType> NeighborhoodType;

  typedef ListNode<IndexType>            ListNodeType;
  typedef ObjectStore<ListNodeType>      ListNodeStorageType;
  typedef SparseFieldLayer<ListNodeType> ListType;
  typedef typename ListType::Pointer     ListPointerType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);  
    
  /** Typedef to describe the output image region type. */
  typedef typename TOutputImage::RegionType OutputImageRegionType;
    
  /** Run-time type information (and related methods). */
  itkTypeMacro(CannyEdgeDetectionRecursiveGaussianImageFilter, ImageToImageFilter);
  
  /** ImageDimension constant    */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);
  
  /** Typedef of double containers */
  typedef FixedArray<double, itkGetStaticConstMacro(ImageDimension)> ArrayType;

  /** Smoothing parameters for the Gaussian filter. */
  itkSetMacro(Sigma, double);
  itkGetMacro(Sigma, double);
  
  /* Set the Threshold value for detected edges. */
  void SetThreshold(const OutputImagePixelType th)
    {
    this->m_Threshold = th;
    this->m_UpperThreshold = m_Threshold;
    this->m_LowerThreshold = m_Threshold/2.0;
    itkLegacyReplaceBodyMacro(SetThreshold, 2.2, SetUpperThreshold);
    }
  
  OutputImagePixelType GetThreshold(OutputImagePixelType th) 
    {
    itkLegacyReplaceBodyMacro(GetThreshold, 2.2, GetUpperThreshold);
    return this->m_Threshold; 
    }

  ///* Set the Threshold value for detected edges. */
  itkSetMacro(UpperThreshold, OutputImagePixelType );
  itkGetMacro(UpperThreshold, OutputImagePixelType);

  itkSetMacro(LowerThreshold, OutputImagePixelType );
  itkGetMacro(LowerThreshold, OutputImagePixelType);

  /* Set the Thresholdvalue for detected edges. */
  itkSetMacro(OutsideValue, OutputImagePixelType);
  itkGetMacro(OutsideValue, OutputImagePixelType);
  
  OutputImageType * GetNonMaximumSuppressionImage()
    {
    return this->m_MultiplyImageFilter->GetOutput();
    }

  /** CannyEdgeDetectionRecursiveGaussianImageFilter needs a larger input requested
   * region than the output requested region ( derivative operators, etc).  
   * As such, CannyEdgeDetectionRecursiveGaussianImageFilter needs to provide an implementation
   * for GenerateInputRequestedRegion() in order to inform the 
   * pipeline execution model.
   *
   * \sa ImageToImageFilter::GenerateInputRequestedRegion()  */  
  virtual void GenerateInputRequestedRegion() throw(InvalidRequestedRegionError);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputHasNumericTraitsCheck,
    (Concept::HasNumericTraits<InputImagePixelType>));
  itkConceptMacro(OutputHasNumericTraitsCheck,
    (Concept::HasNumericTraits<OutputImagePixelType>));
  itkConceptMacro(SameDimensionCheck,
    (Concept::SameDimension<ImageDimension, OutputImageDimension>));
  itkConceptMacro(InputIsFloatingPointCheck,
    (Concept::IsFloatingPoint<InputImagePixelType>));
  itkConceptMacro(OutputIsFloatingPointCheck,
    (Concept::IsFloatingPoint<OutputImagePixelType>));
  /** End concept checking */
#endif

protected:
  CannyEdgeDetectionRecursiveGaussianImageFilter();
  CannyEdgeDetectionRecursiveGaussianImageFilter(const Self&) {}
  void PrintSelf(std::ostream& os, Indent indent) const;

  void GenerateData();

  typedef SmoothingRecursiveGaussianImageFilter<InputImageType, OutputImageType>
                                                      GaussianImageFilterType;
  typedef LaplacianRecursiveGaussianImageFilter<InputImageType, OutputImageType>
                                                      LaplacianImageFilterType;

  typedef MultiplyImageFilter< OutputImageType, 
              OutputImageType, OutputImageType>       MultiplyImageFilterType;

private:
  virtual ~CannyEdgeDetectionRecursiveGaussianImageFilter(){};

  /** Thread-Data Structure   */
  struct CannyThreadStruct
    {
    CannyEdgeDetectionRecursiveGaussianImageFilter *Filter;
    };

  /** This allocate storage for m_UpdateBuffer, m_UpdateBuffer1 */
  void AllocateUpdateBuffer();

  /** Implement hysteresis thresholding */
  void HysteresisThresholding();

  /** Edge linking funciton */
  void FollowEdge(IndexType index);

  /** Check if the index is in bounds or not */
  bool InBounds(IndexType index);
  
  /** The variance of the Gaussian Filter used in this filter */
  double m_Sigma;

  /** The maximum error of the gaussian blurring kernel in each dimensional
   * direction.  */
  ArrayType m_MaximumError;

  /** Upper threshold value for identifying edges. */
  OutputImagePixelType m_UpperThreshold;  //should be float here?
  
  /** Lower threshold value for identifying edges. */
  OutputImagePixelType m_LowerThreshold; //should be float here?

  /** Threshold value for identifying edges. */
  OutputImagePixelType m_Threshold;

  /** "Background" value for use in thresholding. */
  OutputImagePixelType m_OutsideValue;

  /** Update buffers used during calculation of multiple steps */
  typename OutputImageType::Pointer  m_UpdateBuffer1;

  /** Gaussian filter to smooth the input image  */
  typename GaussianImageFilterType::Pointer m_GaussianFilter;

  typename LaplacianImageFilterType::Pointer m_LaplacianFilter;
  
  /** Multiply image filter to multiply with the zero crossings of the second
   *  derivative.  */
  typename MultiplyImageFilterType::Pointer m_MultiplyImageFilter;
  
  std::slice  m_ComputeCannyEdgeSlice[ImageDimension];

  unsigned long m_Stride[ImageDimension];
  unsigned long m_Center;

  typename ListNodeStorageType::Pointer m_NodeStore;
  ListPointerType                       m_NodeList;

};

} //end of namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkCannyEdgeDetectionRecursiveGaussianImageFilter2.txx"
#endif
  
#endif
