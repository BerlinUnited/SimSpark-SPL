#ifndef BASENODE_H__
#define BASENODE_H__

/*	\class BaseNode
	$Id: basenode.h,v 1.2 2003/08/22 22:43:16 rollmark Exp $

	BaseNode

	The base class for all nodes which are part of the scene hierarchy.

	Notable changes versus the AGSPT SceneGraph SGBaseNode:

		- Hierarchy functionality (children, naming, etc..) is inherited from zeitgeist::Node
		- Does NOT have an explicit local and world transform

	NOTE:

	HISTORY:
		05.11.02 - MK
			- Initial version

	TODO:

	TOFIX:
*/

#include <salt/matrix.h>
#include <salt/bounds.h>
#include <zeitgeist/node.h>

namespace kerosin
{

class Scene;

class BaseNode : public zeitgeist::Node
{
	//
	// Functions
	//
public:
	BaseNode();
	~BaseNode();

	// transformation related

	//! return the local transform of this node (default: returns identity)
	virtual const salt::Matrix&	GetLocalTransform()	const;
	//! return the world transform of this node (default: returns parents world transform)
	virtual const salt::Matrix&	GetWorldTransform()	const;
	//! set the local transform of this node (default: ignored)
	virtual void SetLocalTransform(const salt::Matrix &transform);
	//! set the world transform of this node (default: ignored)
	virtual void SetWorldTransform(const salt::Matrix &transform);

	// bounding box related

	//! compute the local bounding box of the node
	virtual void	ComputeBoundingBox();
	//! return the world bounding box of this node
	const salt::AABB3&	GetWorldBoundingBox() const	{	return mWorldBoundingBox;		}

	// scene graph update passes

	//! update internal state before physics calculation
	void PrePhysicsUpdate(float deltaTime);
	//! update internal state after physics calculation
	void PostPhysicsUpdate();
	//! update hierarchical date (position, bounding volumes, etc..)
	void UpdateHierarchy();

	// scene graph rendering
	//! render node
	void Render();

	void RenderAmbient();

	//! move up the hierarchy, until it finds a scene
	boost::shared_ptr<Scene>	GetScene();

	//! debug mode controls
	void EnableDebugMode();
	void DisableDebugMode();

private:
	//! update internal state before physics calculation
	virtual void PrePhysicsUpdateInternal(float deltaTime);
	//! update internal state after physics calculation
	virtual void PostPhysicsUpdateInternal();
	//! update hierarchical date (position, bounding volumes, etc..)
	virtual void UpdateHierarchyInternal();
	//! render node
	virtual void RenderInternal();
	//! render node
	virtual void RenderAmbientInternal();

	//
	// Members
	//
protected:
	//! identity matrix
	static salt::Matrix		mIdentityMatrix;
	//! debug mode (for additional visualization)
	bool					mDebugMode;

	//! local bounding box
	salt::AABB3				mLocalBoundingBox;
	//! world bounding box
	salt::AABB3				mWorldBoundingBox;
};

DECLARE_CLASS(BaseNode);

} //namespace kerosin
#endif //BASENODE_H__
