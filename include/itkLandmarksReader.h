/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkLandmarksReader.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef itkLandmarksReader_h
#define itkLandmarksReader_h

#include "itkProcessObject.h"
#include "itkImage.h"
#include "itkLandmarkSpatialObject.h"
#include "itkSpatialObjectReader.h"

namespace itk
{

/** \class LandmarksReader
 * \brief Class that reads a file containing spatial object landmarks.
 *
 * A LandmarkSpatialObject is produced as output.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class ITK_EXPORT LandmarksReader : public ProcessObject
{
public:
  /** Standard class type alias. */
  using Self = LandmarksReader;
  using Superclass = ProcessObject;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(LandmarksReader, ProcessObject);

  /** Dimension of the space */
  static constexpr unsigned int Dimension = NDimension;

  /** Type of spatialObject that will be passed as input and output of this
   * segmentation method. */
  using SpatialObjectType = LandmarkSpatialObject< NDimension >;
  using SpatialObjectPointer = typename SpatialObjectType::Pointer;

  /** Output data that carries the feature in the form of a
   * SpatialObject. */
  const SpatialObjectType * GetOutput() const;

  /** Set / Get the input filename */
  itkSetStringMacro( FileName );
  itkGetStringMacro( FileName );

protected:
  LandmarksReader();
  ~LandmarksReader() override;
  void PrintSelf(std::ostream& os, Indent indent) const override;

  void GenerateData() override;

private:
  LandmarksReader(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented


  using SpatialObjectReaderType = SpatialObjectReader< NDimension, unsigned short >;
  using SpatialObjectReaderPointer = typename SpatialObjectReaderType::Pointer;
  using SceneType = typename SpatialObjectReaderType::SceneType;
  using ObjectListType = typename SceneType::ObjectListType;

  std::string                     m_FileName;
  SpatialObjectReaderPointer      m_SpatialObjectReader;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
# include "itkLandmarksReader.hxx"
#endif

#endif
