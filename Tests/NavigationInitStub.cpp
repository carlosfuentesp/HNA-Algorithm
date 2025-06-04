#include "Navigation.h"
#include <cstring>

void Navigation::init()
{
    for(int i = 0; i < numGraphs; i++)
    {
        graphs[i].nodes->DestroyIntraEdge();
        graphs[i].nodes->DestroyEdge();
        graphs[i].Destroy();
    }

    numGraphs = 0;
    graphs = (Graph*)dtAlloc(sizeof(Graph)*levels, DT_ALLOC_PERM);
    memset(graphs, 0, sizeof(Graph)*levels);
    numLevel = 0;

    m_nodePool = 0;
    m_openList = 0;
    if (!m_nodePool || m_nodePool->getMaxNodes() < maxNodes)
    {
        if (m_nodePool)
        {
            m_nodePool->~dtNodePool();
            dtFree(m_nodePool);
            m_nodePool = 0;
        }
        m_nodePool = new (dtAlloc(sizeof(dtNodePool), DT_ALLOC_PERM)) dtNodePool(maxNodes, dtNextPow2(maxNodes/4));
    }
    else
    {
        m_nodePool->clear();
    }

    if (!m_openList || m_openList->getCapacity() < maxNodes)
    {
        if (m_openList)
        {
            m_openList->~dtNodeQueue();
            dtFree(m_openList);
            m_openList = 0;
        }
        m_openList = new (dtAlloc(sizeof(dtNodeQueue), DT_ALLOC_PERM)) dtNodeQueue(maxNodes);
    }
    else
    {
        m_openList->clear();
    }

    refBase = m_navMesh->getPolyRefBase(tile);
}
