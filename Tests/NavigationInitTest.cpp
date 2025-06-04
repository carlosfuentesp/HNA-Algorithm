#define log2 __log2
#define private public
#include "Navigation.h"
#undef private
#include "DetourAlloc.h"
#include <set>
#include <cstdio>
#include <cstring>

static std::set<void*> freed;

static void* testAlloc(int size, dtAllocHint)
{
    return malloc(size);
}

static void testFree(void* ptr)
{
    freed.insert(ptr);
    free(ptr);
}

int main()
{
    dtAllocSetCustom(testAlloc, testFree);

    dtNavMesh* nav = dtAllocNavMesh();
    dtNavMeshParams params;
    memset(&params, 0, sizeof(params));
    params.orig[0] = params.orig[1] = params.orig[2] = 0;
    params.tileWidth = 1.0f;
    params.tileHeight = 1.0f;
    params.maxTiles = 1;
    params.maxPolys = 8;
    if (dtStatusFailed(nav->init(&params)))
    {
        printf("navmesh init failed\n");
        return 1;
    }

    dtMeshTile* tile = nav->getTile(0);

    Navigation navg;
    navg.levels = 1;
    navg.numGraphs = 1;
    navg.graphs = (Graph*)dtAlloc(sizeof(Graph)*navg.levels, DT_ALLOC_PERM);
    memset(navg.graphs, 0, sizeof(Graph)*navg.levels);

    Graph& g = navg.graphs[0];
    g.Init(2);
    g.AddNode(0);
    g.AddNode(1);
    g.InitEdge(0,1);
    g.InitEdge(1,1);
    float pos[3] = {0,0,0};
    g.AddEdge(0,1,pos,0,0);
    g.AddEdge(1,0,pos,0,0);
    g.nodes[0].InitIntraEdge();
    g.nodes[1].InitIntraEdge();

    void* edges0 = g.nodes[0].edges;
    void* edges1 = g.nodes[1].edges;
    void* intra0 = g.nodes[0].intraEdges;
    void* intra1 = g.nodes[1].intraEdges;
    void* nodesMem = g.nodes;

    navg.tile = tile;
    navg.m_navMesh = nav;

    navg.init();

    bool ok = freed.count(edges0) && freed.count(edges1) &&
              freed.count(intra0) && freed.count(intra1) &&
              freed.count(nodesMem);

    printf("edge memory freed: %s\n", ok ? "true" : "false");

    dtFreeNavMesh(nav);
    return ok ? 0 : 1;
}
