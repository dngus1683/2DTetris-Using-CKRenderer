
#include "Precompiled.h"
#include "SoftRenderer.h"
#include <random>
using namespace CK::DD;

// ���ڸ� �׸��� �Լ�
void SoftRenderer::DrawGizmo2D()
{
	auto& r = GetRenderer();
	const auto& g = Get2DGameEngine();

	// �׸��� ����
	LinearColor gridColor(LinearColor(0.8f, 0.8f, 0.8f, 0.3f));

	// ���� ���� ���
	Vector2 viewPos = g.GetMainCamera().GetTransform().GetPosition();
	Vector2 extent = Vector2(_ScreenSize.X * 0.5f, _ScreenSize.Y * 0.5f);

	// ���� �ϴܿ������� ���� �׸���
	int xGridCount = _ScreenSize.X / _Grid2DUnit;
	int yGridCount = _ScreenSize.Y / _Grid2DUnit;

	// �׸��尡 ���۵Ǵ� ���ϴ� ��ǥ �� ���
	Vector2 minPos = viewPos - extent;
	Vector2 minGridPos = Vector2(ceilf(minPos.X / (float)_Grid2DUnit), ceilf(minPos.Y / (float)_Grid2DUnit)) * (float)_Grid2DUnit;
	ScreenPoint gridBottomLeft = ScreenPoint::ToScreenCoordinate(_ScreenSize, minGridPos - viewPos);

	for (int ix = 0; ix < xGridCount; ++ix)
	{
		r.DrawFullVerticalLine(gridBottomLeft.X + ix * _Grid2DUnit, gridColor);
	}

	for (int iy = 0; iy < yGridCount; ++iy)
	{
		r.DrawFullHorizontalLine(gridBottomLeft.Y - iy * _Grid2DUnit, gridColor);
	}

	ScreenPoint worldOrigin = ScreenPoint::ToScreenCoordinate(_ScreenSize, -viewPos);
	r.DrawFullHorizontalLine(worldOrigin.Y, LinearColor::Red);
	r.DrawFullVerticalLine(worldOrigin.X, LinearColor::Green);
}

// ���� ������Ʈ ���
static const std::string Tetromino[5] = { "I", "L", "O", "S", "T" };

// ���� �� �ε��� ����ϴ� �Լ�
void SoftRenderer::LoadScene2D()
{
	// ���� �� �ε����� ����ϴ� ��� �� �ֿ� ���۷���
	auto& g = Get2DGameEngine();

}

// ���� ������ ������ ������ �����ϴ� ����


// ���� ������ ����ϴ� �Լ�
void SoftRenderer::Update2D(float InDeltaSeconds)
{
	// ���� �������� ����ϴ� ��� �� �ֿ� ���۷���
	auto& g = Get2DGameEngine();
	const InputManager& input = g.GetInputManager();

	// ���� ������ ���� ����
	static float FallingSpeed = 0.f;

}



void MakeOneSquare(static const float StartX, static const float StartY, static const float SquareLength, std::vector<Vector2>& Square)
{
	for (int i = StartX; i < StartX + SquareLength; i++)
	{
		for (int j = StartY; j < StartY + SquareLength; j++)
		{
			Square.push_back(Vector2(i, j));
		}
	}
}

LinearColor TetrominoColor[5] = {LinearColor::Blue, LinearColor::Green, LinearColor::Red, LinearColor::Yellow, LinearColor::DimGray};
static const float StartX[5][4] = {
	{ 0.f, 20.f, 40.f, 60.f },
	{ 0.f, 20.f, 20.f, 40.f },
	{ 0.f, 20.f, 40.f, 40.f },
	{ 0.f, 20.f, 20.f, 40.f },
	{ 0.f, 20.f, 20.f, 00.f }
};
static const float StartY[5][4] = {
	{ 00.f, 00.f, 00.f, 00.f },
	{ 00.f, 00.f, 20.f, 00.f },
	{ 00.f, 00.f, 00.f, 20.f },
	{ 00.f, 00.f, 20.f, 20.f },
	{ 00.f, 00.f, 20.f, 20.f }
};
int MakeTetromino(static const int mode, std::vector<Vector2>& Square)
{
	for (int i = 0; i < 4; ++i)
	{
		MakeOneSquare(StartX[mode][i], StartY[mode][i], 20.f, Square);
	}

	return mode;
}

// ������ ������ ����ϴ� �Լ�
void SoftRenderer::Render2D()
{
	// ������ �������� ����ϴ� ��� �� �ֿ� ���۷���
	auto& r = GetRenderer();
	const auto& g = Get2DGameEngine();

	// ��濡 ���� �׸���
	DrawGizmo2D();

	// ������ ������ ���� ����
	int mode = 4;
	std::vector<Vector2> Square;
	MakeTetromino(mode, Square);

	for (auto v : Square)
	{
		r.DrawPoint(v, TetrominoColor[mode]);
	}
}

// �޽ø� �׸��� �Լ�
void SoftRenderer::DrawMesh2D(const class DD::Mesh& InMesh, const Matrix3x3& InMatrix, const LinearColor& InColor)
{
}

// �ﰢ���� �׸��� �Լ�
void SoftRenderer::DrawTriangle2D(std::vector<DD::Vertex2D>& InVertices, const LinearColor& InColor, FillMode InFillMode)
{
}
