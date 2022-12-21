# The board
### Map
hexes are stored in a 2d array with a width and height.
Given a tileId, neighbors are found as follow

#define id_bottomleft(id) id+boardWidth-1 + (id/boardWidth+1)%2
#define id_bottomright(id) id+boardWidth + (id/boardWidth+1)%2
#define id_right(id) id+1
#define id_left(id) id-1
#define id_topleft(id) id-boardWidth-1 + (id/boardWidth-1)%2
#define id_topright(id) id-boardWidth + (id/boardWidth-1)%2

And less usefully since x and y are confusing:

#define bottomleft(x,y) x-1+y%2 + (y+1)*boardWidth
#define bottomright(x,y) x+y%2 + (y+1)*boardWidth
#define right(x,y) x+1 + y*boardWidth
#define left(x,y) x-1 + y*boardWidth
#define topleft(x,y) x-1+y%2 + (y-1)*boardWidth
#define topright(x,y) x+y%2 + (y-1)*boardWidth

### Roads and buildings
Each hex points to 3 road placements and 3 building placements,
building places starting from the leftmost vertex going clockwise
edge starting from the topleft going clockwise

###  Harbours / Ports
