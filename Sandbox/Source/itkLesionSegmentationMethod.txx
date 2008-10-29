/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkLesionSegmentationMethod.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLesionSegmentationMethod_txx
#define __itkLesionSegmentationMethod_txx

#include "itkLesionSegmentationMethod.h"


namespace itk
{

/*
 * Constructor
 */
template <unsigned int NDimension>
LesionSegmentationMethod<NDimension>
::LesionSegmentationMethod()
{
  this->SetNumberOfRequiredOutputs( 1 );  // for the Transform
}


/*
 * Destructor
 */
template <unsigned int NDimension>
LesionSegmentationMethod<NDimension>
::~LesionSegmentationMethod()
{
}


/*
 * Add a feature generator that will compute the Nth feature to be passed to
 * the segmentation module.
 */
template <unsigned int NDimension>
void
LesionSegmentationMethod<NDimension>
::AddFeatureGenerator( FeatureGeneratorType * generator ) 
{
  this->m_FeatureGenerators.push_back( generator );
}


/*
 * PrintSelf
 */
template <unsigned int NDimension>
void
LesionSegmentationMethod<NDimension>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << "Region of Interest " << this->m_RegionOfInterest.GetPointer() << std::endl;
  os << "Initial Segmentation " << this->m_InitialSegmentation.GetPointer() << std::endl;
  os << "Segmentation Module " << this->m_SegmentationModule.GetPointer() << std::endl;

  os << "Feature generators = ";
  
  FeatureGeneratorConstIterator gitr = this->m_FeatureGenerators.begin();
  FeatureGeneratorConstIterator gend = this->m_FeatureGenerators.end();

  while( gitr != gend )
    {
    os << gitr->GetPointer() << std::endl;
    ++gitr;
    }

}


/*
 * Generate Data
 */
template <unsigned int NDimension>
void
LesionSegmentationMethod<NDimension>
::GenerateData()
{
  this->UpdateAllFeatureGenerators();
}


/*
 * Update feature generators
 */
template <unsigned int NDimension>
void
LesionSegmentationMethod<NDimension>
::UpdateAllFeatureGenerators()
{
  FeatureGeneratorIterator gitr = this->m_FeatureGenerators.begin();
  FeatureGeneratorIterator gend = this->m_FeatureGenerators.end();

  while( gitr != gend )
    {
    (*gitr)->Update();
    ++gitr;
    }
}


} // end namespace itk

#endif