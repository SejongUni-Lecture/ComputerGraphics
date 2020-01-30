#include <math.h>
#include <vector>

#define NO_EQUAL_VERTEX ((unsigned short)-1)

struct TgVector3
{
    float x, y, z;

    TgVector3(){}
    TgVector3(float a, float b, float c)
    {
        x = a;
        y = b;
        z = c;
    }
    TgVector3 Normalize() const
    {
        TgVector3 c;
        float length = sqrtf(x*x+y*y+z*z);
        c.x = x/length;
        c.y = y/length;
        c.z = z/length;
        return c;
    }

    float Dot (TgVector3 const& a) const
    {
        return x*a.x + y*a.y + z*a.z;
    }

    TgVector3 Cross(TgVector3 const& a) const
    {
        return TgVector3(y*a.z - z*a.y, z*a.x - x*a.z, x*a.y - y*a.x);
    }

    TgVector3& operator+=(const TgVector3& a)
    {
        x += a.x;
        y += a.y;
        z += a.z;

        return *this;
    }


    TgVector3 operator-(TgVector3 const & a) const
    {
        TgVector3 c;
        c.x = x - a.x;
        c.y = y - a.y;
        c.z = z - a.z;
        return c;
    }

    TgVector3 operator+(const TgVector3& a) const
    {
        TgVector3 c;
        c.x = x + a.x;
        c.y = y + a.y;
        c.z = z + a.z;
        return c;
    }

    TgVector3 operator/(float a) const
    {
        TgVector3 c;
        c.x = x / a;
        c.y = y / a;
        c.z = z / a;
        return c;
    }
};


class TgEqualVertexIdx
{
public:
    unsigned short						mPrevIdx;
    unsigned short						mNextIdx;
    TgEqualVertexIdx()
    {
        mPrevIdx = NO_EQUAL_VERTEX;
        mNextIdx = NO_EQUAL_VERTEX;
    }
};


class TgMesh
{

public:

    // indices.
    std::vector<unsigned short>				mIndices;

    // vertices
    std::vector<TgVector3>					mVertices;								// position.
    std::vector<TgVector3>					mNormals;

    // equal vertices for position.
    std::vector<TgEqualVertexIdx>			mEqualVertices;

    void ComputeVertexNormalsWithFaceThreshold(float faceThresholdRadian);
};


class FaceVertexNormal
{
public:
    int mFaceNormalCount;
    TgVector3 mVertexNormal;
    FaceVertexNormal* mSharedVertexNext;
    //std::vector<int> mIndexBuffer;
    int mSourceVertexIdx;
    int mArrangedVetexIdx;

public:
    FaceVertexNormal()
    {
        mVertexNormal = TgVector3(0.0f, 0.0f, 0.0f);
        mSharedVertexNext = 0;
        mFaceNormalCount = 0;
        mSourceVertexIdx = 0;
        mArrangedVetexIdx = 0;
    }

    ~FaceVertexNormal()
    {
        if (mSharedVertexNext)
        {
            delete mSharedVertexNext;
            mSharedVertexNext = 0;
        }
    }

    FaceVertexNormal* AddFaceNormal(TgVector3 const& faceNormal, float thresholdCosine, int sourceVertexIdx)
    {
        if (mFaceNormalCount == 0)
        {
            mVertexNormal = faceNormal;
            mSourceVertexIdx = sourceVertexIdx;
            mFaceNormalCount++;
            return this;
        }
        else
        {
            float dot = mVertexNormal.Normalize().Dot(faceNormal.Normalize());
            if (0 < dot && 1.0f - dot < thresholdCosine)
            {
                mVertexNormal += faceNormal;
                mSourceVertexIdx = sourceVertexIdx;
                mFaceNormalCount++;
                return this;
            }
            else
            {
                if (mSharedVertexNext == 0)
                    mSharedVertexNext = new FaceVertexNormal;

                return mSharedVertexNext->AddFaceNormal(faceNormal, thresholdCosine, sourceVertexIdx);
            }
        }

        return 0;
    }

    int ArrangeVetexIdx(int baseIdx)	// return next reranging vertex idx
    {
        if (mFaceNormalCount == 0)
            return baseIdx;

        mArrangedVetexIdx = baseIdx;
        if (mSharedVertexNext)
            return mSharedVertexNext->ArrangeVetexIdx(baseIdx +1);

        return baseIdx + 1;
    }

    void NormalizeVertexNormal()
    {
        if (mFaceNormalCount == 0)
            return;

        mVertexNormal = mVertexNormal.Normalize();
        if (mSharedVertexNext)
            return mSharedVertexNext->NormalizeVertexNormal();
    }
};




int TgFindEqualVertexRootIdx(std::vector<TgEqualVertexIdx> const& equalVertices, int vertexId)
{
    for (;;)
    {
        unsigned short prevIdx = equalVertices[vertexId].mPrevIdx;
        if (prevIdx == NO_EQUAL_VERTEX)
            return vertexId;

        vertexId = prevIdx;
    }

    return -1;
}


void TgMesh::ComputeVertexNormalsWithFaceThreshold(float faceThresholdRadian)
{
    if (mIndices.empty() == true)
        return;

    int faceCnt = (int)mIndices.size() / 3;

    // make face normal
    std::vector<TgVector3> faceNormals;
    faceNormals.resize(faceCnt);
    for (int i = 0; i < faceCnt; ++i)
    {
        TgVector3 const& v0 = mVertices[mIndices[i*3+0]];
        TgVector3 const& v1 = mVertices[mIndices[i*3+1]];
        TgVector3 const& v2 = mVertices[mIndices[i*3+2]];
        TgVector3 normalVector = (v1-v0).Cross(v2-v0);
        TgVector3 unitNormal = normalVector.Normalize();
        TgVector3 faceCenter = (v0 + v1 + v2)/3.0f;

        faceNormals[i] = unitNormal;
    }

    // initialize represent vertex normals
    std::vector<FaceVertexNormal> faceVertexNormals;
    faceVertexNormals.resize(mVertices.size());

    // add face normal to vertex normal.
    std::vector<FaceVertexNormal*> newFaceIndices;
    newFaceIndices.reserve(mIndices.size());
    float faceThresholdCosine = cos(faceThresholdRadian);
    for (int i = 0; i < (int)mIndices.size(); i += 3)
    {
        for (int j = 0; j < 3; ++j)
        {
            int vertexIdx = mIndices[i+j];
            int faceIdx = i/3;
            int rootIdx = TgFindEqualVertexRootIdx(mEqualVertices, vertexIdx);
            TgVector3 const& faceNormal = faceNormals[faceIdx];
            FaceVertexNormal* addedGroup = 
                faceVertexNormals[rootIdx].AddFaceNormal(faceNormal, faceThresholdCosine, vertexIdx);
            newFaceIndices.push_back(addedGroup);
        }
    }

    // re-arrange vertex id
    int arrangingVertexIdx = 0;
    int newVertexCount = 0;
    for (int i = 0; i < (int)faceVertexNormals.size(); ++i)
    {
        FaceVertexNormal* faceVertexNormal = &faceVertexNormals[i];
        arrangingVertexIdx = faceVertexNormal->ArrangeVetexIdx(arrangingVertexIdx);
    }
    newVertexCount = arrangingVertexIdx;

    // normalize vertex normal.
    for (int i = 0; i < (int)faceVertexNormals.size(); ++i)
    {
        FaceVertexNormal* faceVertexNormal = &faceVertexNormals[i];
        faceVertexNormal->NormalizeVertexNormal();
    }

    // re-arrange vertex id.
    std::vector<TgVector3> newVertices;
    std::vector<TgVector3> newNormals;
    newVertices.reserve(newVertexCount);
    newNormals.reserve(newVertexCount);
    for (int i = 0; i < (int)faceVertexNormals.size(); ++i)
    {
        FaceVertexNormal* faceVertexNormal = &faceVertexNormals[i];
        for (;faceVertexNormal;)
        {
            if (0 < faceVertexNormal->mFaceNormalCount)
            {
                int sourceVertexIdx = faceVertexNormal->mSourceVertexIdx;
                newVertices.push_back(mVertices[sourceVertexIdx]);
                newNormals.push_back(faceVertexNormal->mVertexNormal);
            }
            faceVertexNormal = faceVertexNormal->mSharedVertexNext;
        }
    }
    mVertices.swap(newVertices);
    mNormals.swap(newNormals);

    // re-arrange index buffer.
    std::vector<unsigned short> newIndices;
    newIndices.resize(newFaceIndices.size());
    for (int i = 0; i < (int)newFaceIndices.size(); ++i)
    {
        newIndices[i] = newFaceIndices[i]->mArrangedVetexIdx;
    }
    mIndices.swap(newIndices);
}


