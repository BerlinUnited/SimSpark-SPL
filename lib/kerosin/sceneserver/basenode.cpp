#include "basenode.h"
#include "scene.h"
#include "../openglserver/openglserver.h"

using namespace boost;
using namespace kerosin;
using namespace salt;
using namespace zeitgeist;

salt::Matrix BaseNode::mIdentityMatrix;

BaseNode::BaseNode() :
zeitgeist::Node(), mDebugMode(false)
{
	// a bit redundant, but somehow we have to ensure that this is ALWAYS set
	mIdentityMatrix.Identity();
}

BaseNode::~BaseNode()
{
}

const salt::Matrix& BaseNode::GetLocalTransform() const
{
	return mIdentityMatrix;
}

const salt::Matrix& BaseNode::GetWorldTransform() const
{
	shared_ptr<BaseNode> parent = shared_static_cast<BaseNode>(make_shared(mParent));

	// no parent, return identity
	if (parent.get() == NULL)
		return mIdentityMatrix;
	else
		return parent->GetWorldTransform();
}

void BaseNode::SetLocalTransform(const salt::Matrix &transform)
{
}

void BaseNode::SetWorldTransform(const salt::Matrix &transform)
{
}

/*!
	This routine is responsible for updating the local bounding box of the
	node. The default behavior is to treat the node as a point!

	Please note that this is always called from Compute()!
*/
void BaseNode::ComputeBoundingBox()
{
	mLocalBoundingBox.minVec.Set(0,0,0);
	mLocalBoundingBox.maxVec.Set(0,0,0);
}

void BaseNode::PrePhysicsUpdate(float deltaTime)
{
	// perform update on hierarchy
	for (TLeafList::iterator i = begin(); i!= end(); ++i)
	{
		if ((*i)->GetClass()->Supports("BaseNode"))
			(shared_static_cast<BaseNode>(*i))->PrePhysicsUpdate(deltaTime);
	}

	// do the internal update ... can be overridden by derived classes to add
	// custom behavior
	PrePhysicsUpdateInternal(deltaTime);

	ComputeBoundingBox();
}

void BaseNode::PostPhysicsUpdate()
{
	// perform update on hierarchy
	for (TLeafList::iterator i = begin(); i!= end(); ++i)
	{
		if ((*i)->GetClass()->Supports("BaseNode"))
			(shared_static_cast<BaseNode>(*i))->PostPhysicsUpdate();
	}

	// do the internal update ... can be overridden by derived classes to add
	// custom behavior
	PostPhysicsUpdateInternal();
}

void BaseNode::UpdateHierarchy()
{
	// do the internal update ... can be overridden by derived classes to add
	// custom behavior
	UpdateHierarchyInternal();

	// generate the bounding volume of this node 
	Matrix worldTransform = GetWorldTransform();
	mWorldBoundingBox = mLocalBoundingBox;
	mWorldBoundingBox.TransformBy(worldTransform);

	// perform update on hierarchy
	for (TLeafList::iterator i = begin(); i!= end(); ++i)
	{
		if ((*i)->GetClass()->Supports("BaseNode"))
		{
			shared_ptr<BaseNode> node = shared_static_cast<BaseNode>(*i);
			node->UpdateHierarchy();
			// here we merge our world bounding volume with the child volumes
			mWorldBoundingBox.Encapsulate(node->GetWorldBoundingBox());
		}
	}
}

void BaseNode::Render()
{
	// do the internal update ... can be overridden by derived classes to add
	// custom behavior
	glPushMatrix();
	glMultMatrixf(GetWorldTransform().m);

	RenderInternal();

	glPopMatrix();
	
	// perform update on hierarchy
	for (TLeafList::iterator i = begin(); i!= end(); ++i)
	{
		if ((*i)->GetClass()->Supports("BaseNode"))
			(shared_static_cast<BaseNode>(*i))->Render();
	}
}

void BaseNode::RenderAmbient()
{
	// do the internal update ... can be overridden by derived classes to add
	// custom behavior
	glPushMatrix();
	glMultMatrixf(GetWorldTransform().m);

	RenderAmbientInternal();

	glPopMatrix();
	
	// perform update on hierarchy
	for (TLeafList::iterator i = begin(); i!= end(); ++i)
	{
		if ((*i)->GetClass()->Supports("BaseNode"))
			(shared_static_cast<BaseNode>(*i))->RenderAmbient();
	}
}

boost::shared_ptr<Scene>	BaseNode::GetScene()
{
	shared_ptr<Leaf> leaf = shared_static_cast<Leaf>(make_shared(GetSelf()));

	while(leaf && leaf->GetClass() && !leaf->GetClass()->Supports("Scene"))
	{
		leaf = make_shared(leaf->GetParent());
	}

	if (leaf && leaf->GetClass())
	{
		// class name has to be "Scene"
		return shared_static_cast<Scene>(leaf);
	}
	else
	{
		return shared_ptr<Scene>();
	}
}

void BaseNode::EnableDebugMode()
{
	mDebugMode = true;
}

void BaseNode::DisableDebugMode()
{
	mDebugMode = false;
}

void BaseNode::PrePhysicsUpdateInternal(float deltaTime)
{
}

void BaseNode::PostPhysicsUpdateInternal()
{
}

void BaseNode::UpdateHierarchyInternal()
{
}

void BaseNode::RenderInternal()
{
}

void BaseNode::RenderAmbientInternal()
{
}
