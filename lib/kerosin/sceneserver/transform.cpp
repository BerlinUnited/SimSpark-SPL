#include "transform.h"

using namespace boost;
using namespace kerosin;
using namespace salt;
using namespace zeitgeist;

Transform::Transform() :
BaseNode()
{
	mLocalTransform.Identity();
	mWorldTransform.Identity();

	SetName("transform");
}

Transform::~Transform()
{
}

const salt::Matrix& Transform::GetLocalTransform() const
{
	return mLocalTransform;
}

const salt::Matrix& Transform::GetWorldTransform() const
{
	return mWorldTransform;
}

void Transform::SetLocalTransform(const salt::Matrix &transform)
{
	mLocalTransform = transform;
}

void Transform::SetWorldTransform(const salt::Matrix &transform)
{
	mWorldTransform = transform;
}

void Transform::SetLocalPos(float x, float y, float z)
{
	SetLocalPos(Vector3f(x, y, z));
}

void Transform::SetLocalPos(const salt::Vector3f &pos)
{
	mLocalTransform.Pos() = pos;
	UpdateHierarchyInternal();
}

void Transform::OnLink()
{
	UpdateHierarchyInternal();
}

void Transform::UpdateHierarchyInternal()
{
	shared_ptr<BaseNode> parent = shared_static_cast<BaseNode>(make_shared(mParent));

	// no parent, return identity
	if (parent.get() == NULL)
		mWorldTransform = mLocalTransform;
	else
		mWorldTransform = parent->GetWorldTransform() * mLocalTransform;
}