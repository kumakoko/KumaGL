

/**
 *
 * Generates a simple flat grid mesh which will be used as the water. Very
 * similar to how the "split terrain" was generated in tutorial 55. Each
 * triangle of the mesh has its own 3 vertices so that no vertices are shared
 * between triangles. This allows for the flat-shaded look as the
 * normals/lighting values aren't interpolated across multiple triangles.
 *
 * @author Karl
 *
 */

namespace kgl
{
	class WaterGenerator
	{

	private:
		static const int VERTICES_PER_SQUARE = 3 * 2;// 2 triangles
		static const int VERTEX_SIZE_BYTES = 8 + 4;// x,z position +
		// indicator

		/**
		 * Generates a water mesh of a given size. First the total number of
		 * vertices in the mesh is calculated. Then all the vertex data for the mesh
		 * is generated and stored in a byte[]. A VAO is then created, and the
		 * vertex data is stored in it.
		 *
		 * @param gridCount
		 *            - The number of grid squares along each edge of the mesh.
		 * @param height
		 *            - The height of the water mesh in the world.
		 * @return The newly constructed water mesh.
		 */
	public:
	//	static WaterTile generate(int gridCount, float height);

		/**
		 * Generates all the vertex data for the water mesh. First a byte buffer of
		 * the correct size is initialized, and this will hold all the vertex data
		 * for the entire mesh. It then iterates through all the grid squares in the
		 * mesh and for each one it stores the vertex data for the two triangles in
		 * that square. Therefore, the data for 6 vertices is stored per grid
		 * square.
		 *
		 * @param gridCount
		 *            - The number of grid squares along one edge of the mesh.
		 * @param totalVertexCount
		 *            - The total number of vertices that will be in the mesh.
		 * @return The vertex data (stored as bytes) for the entire mesh.
		 */
	private:
		static void createMeshData(int gridCount, int totalVertexCount,std::vector<uint8_t>& vertices_data);

		/**
		 * Generates all the vertex data for a grid square. The corner positions of
		 * the grid square are calculated and then the data for the two triangles in
		 * this square is stored. Note that the data for the two triangles is stored
		 * separately, meaning that the data for 6 vertices is stored.
		 *
		 * @param col
		 *            - The column number of this grid square in the grid.
		 * @param row
		 *            - The row number of this grid square in the grid.
		 * @param buffer
		 *            - The buffer where all the vertex data is being collected.
		 */
		static void storeGridSquare(int col, int row, const std::vector<uint8_t>& buffer);

		/**
		 * Stores the vertex data for a given triangle of the mesh into the
		 * ByteBuffer. First it is determined which 3 of the vertices of the current
		 * grid square make up the triangle. The indexes for a grid square are as
		 * follows:
		 *
		 * 0 = top left, 1 = bottom left, 2 = top right, 3 = bottom right.
		 *
		 * This is the order that the corner positions are stored in the "cornerPos"
		 * array.
		 *
		 * Once it has been determined which 3 vertices make up the triangle, the
		 * vertex data for those 3 vertices is stored in the ByteBuffer. For each
		 * vertex the x,z position is stored, along with the 4 indicator values.
		 *
		 * @param cornerPos
		 *            - The 4 corner positions for the current grid square, stored
		 *            in the order specified above.
		 * @param buffer
		 *            - The buffer containing all the vertex data for the mesh.
		 * @param left
		 *            - Indicates whether the triangle being stored is the triangle
		 *            on the left or the right of the current grid square.
		 */
		static void storeTriangle(const std::array<glm::vec2,4>& cornerPos, const std::vector<uint8_t>& buffer, bool left);

		/**
		 * Calculates the x,z positions of the 4 corners of a grid square.
		 *
		 * @param col
		 *            - The column number of the grid square.
		 * @param row
		 *            - The row number of the grid square.
		 * @return An array contain 4 positions. Each 2D position is the x,z
		 *         position of one of the corners. The corners are stored in the
		 *         following order: 0 = top left, 1 = bottom left, 2 = top right, 3
		 *         = bottom right
		 */
		static void calculateCornerPositions(int col, int row, std::array<glm::vec2, 4>& vertices);

		/**
		 * Gets the 4 indicator values for a certain vertex. This is done by
		 * calculating the vector from the current vertex to each of the other two
		 * vertices in the current triangle.
		 *
		 * The 3 vertex positions are taken from the "vertexPositions" array, and
		 * then the offset vectors are calculated by subtracting the current vertex
		 * position from the other vertex position.
		 *
		 * The offsets are then stored in an array as bytes (not converted to bytes,
		 * but simply cast to bytes) and returned. The size of each grid square must
		 * be an integer value for this to work, otherwise the offsets wouldn't be
         * able to be represented correctly as bytes.
         *
         * @param currentVertex
         *            - The index of the current vertex in the current grid square
         *            (A number between 0 and 3).
         * @param vertexPositions
         *            - The 4 corner positions of the current grid square, stored in
         *            the following order: 0 = top left, 1 = bottom left, 2 = top
         *            right, 3 = bottom right
         * @param vertex1
         *            - The index of one of the other vertices in the triangle
         *            (number between 0 and 3).
         * @param vertex2
         *            - The index of the other vertex in the triangle (number
         *            between 0 and 3).
         * @return
         */
        static void getIndicators(int currentVertex, const std::vector<glm::vec2>& vertexPositions, int vertex1, int vertex2, std::array<uint8_t, 4>& buffer);
    };

}
