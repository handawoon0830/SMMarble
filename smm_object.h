//
//  smm_object.h
//  SMMarble object
//
//  Created by Juyeop Kim on 2023/11/05.
//

#ifndef smm_object_h
#define smm_object_h

#define SMMNODE_TYPE_LECTURE 		0
#define SMMNODE_TYPE_RESTAURANT 	1
#define SMMNODE_TYPE_LABORTORY 		2
#define SMMNODE_TYPE_HOME 			3
#define SMMNODE_TYPE_GOTOLAB 		4
#define SMMNODE_TYPE_FOODCHANCE 	5
#define SMMNODE_TYPE_FESTIVAL 		6

#define SMMNODE_TYPE_MAX 			7


/* node type :
    lecture,
    restaurant,
    laboratory,
    home,
    experiment,
    foodChance,
    festival
*/


/* grade :
    A+,
    A0,
    A-,
    B+,
    B0,
    B-,
    C+,
    C0,
    C-
*/



//object generation
int smmObj_genNode(void);

//member retrieving


//element to string



#endif /* smm_object_h */
