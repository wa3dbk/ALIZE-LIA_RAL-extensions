/*
This file is part of LIA_RAL which is a set of software based on ALIZE
toolkit for speaker recognition. ALIZE toolkit is required to use LIA_RAL.

LIA_RAL project is a development project was initiated by the computer
science laboratory of Avignon / France (Laboratoire Informatique d'Avignon -
LIA) [http://lia.univ-avignon.fr <http://lia.univ-avignon.fr/>]. Then it
was supported by two national projects of the French Research Ministry:
	- TECHNOLANGUE program [http://www.technolangue.net]
	- MISTRAL program [http://mistral.univ-avignon.fr]

LIA_RAL is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of
the License, or any later version.

LIA_RAL is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with LIA_RAL.
If not, see [http://www.gnu.org/licenses/].

The LIA team as well as the LIA_RAL project team wants to highlight the
limits of voice authentication in a forensic context.
The "Person Authentification by Voice: A Need of Caution" paper
proposes a good overview of this point (cf. "Person
Authentification by Voice: A Need of Caution", Bonastre J.F.,
Bimbot F., Boe L.J., Campbell J.P., Douglas D.A., Magrin-
chagnolleau I., Eurospeech 2003, Genova].
The conclusion of the paper of the paper is proposed bellow:
[Currently, it is not possible to completely determine whether the
similarity between two recordings is due to the speaker or to other
factors, especially when: (a) the speaker does not cooperate, (b) there
is no control over recording equipment, (c) recording conditions are not
known, (d) one does not know whether the voice was disguised and, to a
lesser extent, (e) the linguistic content of the message is not
controlled. Caution and judgment must be exercised when applying speaker
recognition techniques, whether human or automatic, to account for these
uncontrolled factors. Under more constrained or calibrated situations,
or as an aid for investigative purposes, judicious application of these
techniques may be suitable, provided they are not considered as infallible.
At the present time, there is no scientific process that enables one to
uniquely characterize a persones voice or to identify with absolute
certainty an individual from his or her voice.]

Copyright (C) 2004-2010
Laboratoire d'informatique d'Avignon [http://lia.univ-avignon.fr]
LIA_RAL admin [alize@univ-avignon.fr]
Jean-Francois Bonastre [jean-francois.bonastre@univ-avignon.fr]
*/

#if !defined(ALIZE_PrintLLK_cpp)
#define ALIZE_PrintLLK_cpp

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cassert>
#include <cmath>
#include <liatools.h>
#include "PrintLLK.h"

using namespace alize;
using namespace std;

 /// Compute Log Likelihood of the Factor Analysis model
double getLLK(SegCluster &selectedSegments,MixtureGD &model,FeatureServer&fs,Config & config){
        if (verbose) cout << "(FactorAnalysisStat_chan_Sp) Compute Likelihood" << endl;     
        double llk=0.0;
        StatServer _ss(config);
        MixtureGDStat &acc=_ss.createAndStoreMixtureStat(model);     
        Seg *seg;     
        selectedSegments.rewind();     
        while((seg=selectedSegments.getSeg())!=NULL){     
                unsigned long begin=seg->begin()+fs.getFirstFeatureIndexOfASource(seg->sourceName()); 
                fs.seekFeature(begin);
                Feature f;
                for (unsigned long idxFrame=0;idxFrame<seg->length();idxFrame++){
                        fs.readFeature(f); 
                        acc.computeAndAccumulateLLK(f,1.0,TOP_DISTRIBS_NO_ACTION);
                }               
        }             
        llk= acc.getMeanLLK();
        _ss.deleteMixtureStat(acc);
return llk;  
};

int printLLK(Config &config)
{

 bool writeAllFeature=true; // Output a vector for all input vectors (selected and not selected vectors) - DEFAULT=on
 if (config.existsParam("writeAllFeatures")) writeAllFeature=config.getParam("writeAllFeatures").toBool();    // Define if all the feature     (selected or not) should be written

	String modelname = config.getParam("inputModelFilename");
	  String inputFeatureFileName =config.getParam("inputFeatureFilename");          // input feature - could be a simple feature file or a list of filenames
        XLine inputFeatureFileNameList;                                                // The (feature) input filename list
        if (inputFeatureFileName.endsWith(".lst")){                                   // If the file parameter is the name of a XList file
	   XList inputFileNameXList(inputFeatureFileName,config);                   // Read the filename list file
           inputFeatureFileNameList=inputFileNameXList.getAllElements();            // And put the filename in a list if the file is a list of feature filenames
			      }
      else {                                                                         // It was a simple feature file and not a filename list
	          inputFeatureFileNameList.addElement(inputFeatureFileName);                   // add the filename in the list
		    }

	try{

        // read UBM 
        MixtureServer _ms(config);
	//StatServer _ss(config);
        _ms.loadMixtureGD(config.getParam("inputWorldFilename"));
        MixtureGD & UBM=_ms.getMixtureGD((unsigned long) 0);
        //MixtureGDStat &acc=_ss.createAndStoreMixtureStat(UBM);

	unsigned long _vsize=UBM.getVectSize();
	unsigned long _mixsize=UBM.getDistribCount();
        // Loop over the list of feature files
	String *file;
	String labelSelectedFrames;
        unsigned long codeSelectedFrame;
	while ((file=inputFeatureFileNameList.getElement())!= NULL){         
	String & featureFilename=(*file);

	FeatureServer fs(config,featureFilename);
        SegServer segmentsServer;
        LabelServer labelServer;
        initializeClusters(featureFilename,segmentsServer,labelServer,config);
        verifyClusterFile(segmentsServer,fs,config);
	labelSelectedFrames=config.getParam("labelSelectedFrames");
        codeSelectedFrame=labelServer.getLabelIndexByString(labelSelectedFrames);
        SegCluster& selectedSegments=segmentsServer.getCluster(codeSelectedFrame); 

        cout<<"LLK: "<<getLLK(selectedSegments,UBM,fs,config)<<endl;

/*
	// Compute Occupations and Statistics
        acc.resetOcc();
        Seg *seg;
        selectedSegments.rewind();
        String currentSource="";
        while((seg=selectedSegments.getSeg())!=NULL){
                unsigned long begin=seg->begin()+fs.getFirstFeatureIndexOfASource(seg->sourceName());    // Idx of the first frame of the current file in the feature server
                if (currentSource!=seg->sourceName()) {
                currentSource=seg->sourceName();
                if (verbose)cout << "Processing speaker["<<currentSource<<"]"<< endl;
                }

                fs.seekFeature(begin);
                Feature f;

	for (unsigned long idxFrame=0;idxFrame<seg->length();idxFrame++){
                                fs.readFeature(f);
                                acc.computeAndAccumulateOcc(f);
                                RealVector <double> aPost=acc.getOccVect();

				Feature tmpF;
				for(unsigned long k=0;k<_mixsize;k++) {
				tmpF[k]=aPost[k];
				}
}
}
*/
}


	}	
	catch (Exception& e){cout << e.toString().c_str() << endl;}
return 0;
}


#endif //!defined(ALIZE_PrintLLK_cpp)
