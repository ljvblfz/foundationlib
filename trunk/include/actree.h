/************************************************************************/
/*	Copyright to AI INFINITY, Inc.
	Solution designed by Ben Shaw.
	Programmed by Ben Shaw.
	$Reversion 1.0 $ $Date: 2007/01/19 $ Created by Ben Shaw
    $Reversion 1.1 $ $Date: 2009/11/19 $ Modified by Ben Shaw
*/
/************************************************************************/
#include "datatype.h"

typedef void* ACT_HANDLE;

#define ACT_ACN_TYPE 1  /* Abstract Component Node Type */
#define ACT_AGN_TYPE 2  /* Abstract Geometric Node Type */

#define ACT_ERROR              -1
#define ACT_OK                 0
#define ACT_NULL               ((void*) 0)

typedef struct  
{
	MG_HANDLE                       MGHandle;
	MG_REGION                       GeometricRegion;
}ABSTRACT_COMPONENT_INFO,*ABSTRACT_COMPONENT_INFO_PTR;

typedef struct  
{
    /*
    *	0	keyboard
    *	1	mouse
    *	2	screen pad
    */
    int				ActiveDevice;
    /*
    *	0	keyboard:
    *		undefine
    *
    *	1	mouse
    *		Mouse Status:
    *			128 : button up
    *			129 : Left down
    *			130 : Right down
    *			131 : both down
    *			132 : middle down
    *			133 : double click
    *		Mouse Action:
    *			128->128			Mouse motion		0x8080
    *			128->129			Left press down		0x8081
    *			129->128			Left release up		0x8180
    *		128->129&129->128		Left click			0x80818180
    *			129->129			Left drag			0x8181
    *			128->130			Right press down	0x8082
    *			130->128			Right release up	0x8280
    *		128->130&130->128		Right click			0x80828280
    *			130->130			Right drag			0x8282
    *			128->132			Middle press down	0x8084
    *			132->128			Middle release up	0x8480
    *		128->132&132->128		Middle click		0x80848480
    *			132->132			Middle drag			0x8484
    *			129->133			Left double click	0x8185
    *			130->133			Right double click	0x8285
    *
    *	2	screen pad
    *		undefine
    */
    int				ActiveType;
    /*
    *	The first entry of every active array is predefined to 
    *	"Mouse move on".
    */
    /*
    *	Four parameter will deliver
    *	1.Current AC Node pointer
    *	2.Current Function Region Pointer
    *	3.Local relative position coordinate pointer
    *	4.User Interaction Event Pointer
    */
    void(*ActiveFcn)(void*,void*,void*,void*);			/* temporary define */

}ACTION_MAP,*ACTION_MAP_PTR;

/* User Input Event */
typedef struct  
{
    int				ActiveDevice;
    int				VirtualEvent;
    int				KBEvent;
    MOUSE_EVENT_PTR	MouseEvent;
}UI_EVENT,*UI_EVENT_PTR;

typedef struct 
{
    int				Tag;
    LOGICAL			Enable;
    MG_REGION		GeometricRegion;
    int				ActiveMapAmount;
    ACTION_MAP_PTR	ActionFcnMap;
}FCN_REG_INFO,*FCN_REG_INFO_PTR;

/* Initial Abstract Component Tree */
/* Initial children grid */

int insertChild(
				ABSTRACT_COMPONENT_TREE actree
				, ABSTRACT_COMPONENT_NODE_PTR acnode
				);
/*
 *	Delete a Abstract Tree and all its children
 *	If current Abstract Component Node is a shadow object, just delete its
 *	brother, children and itself. Otherwise, if it has grid, delete the 
 *	grid and all its members; If it has brithers delete, all of them; If it 
 *	has children, delete all of them.
 */
void deleteTree(ABSTRACT_COMPONENT_TREE actree);
/*
 *	Delete all the children of a abstract component, include the Children
 *	List and Children Grid.
 */
void delChildren(ABSTRACT_COMPONENT_TREE actree);
/*
 *	Delete a child of the father node, use handle as the match tag.
 *	Delete all of the real and Shadow object.
 *  The input abstract component must be a real object.
 */
int delChild_handle(ABSTRACT_COMPONENT_TREE actree,MG_HANDLE handle);
/*
 *	Delete a child from the children list.
 */
int delChildinList(
				 ABSTRACT_COMPONENT_TREE actree
				,MG_HANDLE handle
				);
/*
 *	Find a Child with Geometric Coordinate	
 */
ABSTRACT_COMPONENT_NODE_PTR findChild_gc(
									  ABSTRACT_COMPONENT_TREE actree
									  , COORDINATE_PTR wp
									  );
/*
 *	Find End Object with World Geometric Coordinate
 *	The function will return a End AC Object and convert
 *	the world coordinate to local relative coordinate
 */
ABSTRACT_COMPONENT_NODE_PTR feogc(
								  ABSTRACT_COMPONENT_TREE actree
								  ,COORDINATE_PTR wp
								 );

/*
 *	Find a abstract component with handle from the Children List (The brothers)
 */
ABSTRACT_COMPONENT_NODE_PTR findachcl(
										 ABSTRACT_COMPONENT_TREE actree,
										 MG_HANDLE handle
										 );
/*
 *	Move a Abstract Component to the top in the grid
 *	This routine will move the Abstract Component to the top of every 
 *	AC list which is registered to the grid. 
 */
int moveACtop_ACGrid(MG_GRID_PTR grid,ABSTRACT_COMPONENT_NODE_PTR ACNodePtr);
/*
 *	Facility resource
 */
/*
 *	Get world absolute region coordinate
 */
MG_REGION getwarc(ABSTRACT_COMPONENT_NODE_PTR ACNodePtr);

