
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

// ���� �� �ε��� ����ϴ� �Լ�
void SoftRenderer::LoadScene2D()
{
	// ���� �� �ε����� ����ϴ� ��� �� �ֿ� ���۷���
	auto& g = Get2DGameEngine();

}

// ���� ������ ������ ������ �����ϴ� ����
Vector2 CurrentPosition = Vector2(0.f, 200.f);
static float FallingTime = 0.f;
static float CurrentRotation = 0.f;

// ���� ������ ����ϴ� �Լ�
void SoftRenderer::Update2D(float InDeltaSeconds)
{
	// ���� �������� ����ϴ� ��� �� �ֿ� ���۷���
	auto& g = Get2DGameEngine();
	const InputManager& input = g.GetInputManager();

	// ���� ������ ���� ����
	static float FallingDistance = -10.f;
	static float MovingSpeed = 300.f;
	Vector2 InputXDirection = Vector2(input.GetAxis(InputAxis::XAxis), input.GetAxis(InputAxis::YAxis));

	FallingTime += InDeltaSeconds;
	if (FallingTime >= 2.f)
	{
		Vector2 DeltaFallingPosition = Vector2(0.f, FallingDistance);
		CurrentPosition += DeltaFallingPosition;
		FallingTime = 0.f;
	}
	
	Vector2 DeltaPosition = InputXDirection * MovingSpeed * InDeltaSeconds;
	

	CurrentPosition += DeltaPosition;
	if (input.IsPressed(InputButton::Space))
	{
		CurrentRotation += 90.f;
	}

}

LinearColor TetrominoColor[5] = {LinearColor::Blue, LinearColor::Green, LinearColor::Red, LinearColor::Yellow, LinearColor::DimGray};

static const float StartX[5][4] = {
	{ -40.f, -20.f, 0.f, 20.f },
	{ -30.f, -10.f, -10.f, 10.f },
	{ -30.f, -10.f, 10.f, 10.f },
	{ -30.f, -10.f, -10.f, 10.f },
	{ -20.f, 0.f, 0.f, -20.f }
};

static const float StartY[5][4] = {
	{ -10.f, -10.f, -10.f, -10.f },
	{ -10.f, -10.f, 10.f, -10.f },
	{ -10.f, -10.f, -10.f, 10.f },
	{ -20.f, -20.f, 0.f, 0.f },
	{ -20.f, -20.f, 0.f, 0.f }
};

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
	static std::random_device rd;
	static std::mt19937 mt(rd());
	static std::uniform_int_distribution<int> RandomMode(0, 4);
	std::vector<Vector2> Square;
	MakeTetromino(4, Square);
	
	// Translation Matrix
	Vector3 TranslationBasis1 = Vector3::UnitX;
	Vector3 TranslationBasis2 = Vector3::UnitY;
	Vector3 TranslationBasis3 = Vector3(CurrentPosition, 1);
	Matrix3x3 Translation(TranslationBasis1, TranslationBasis2, TranslationBasis3);

	// Rotation Matrix
	float sin = 0.f;
	float cos = 0.f;
	Math::GetSinCos(sin, cos, CurrentRotation);
	Vector3 RotationBasis1 = Vector3(cos, sin, 0);
	Vector3 RotationBasis2 = Vector3(-sin, cos, 0);
	Vector3 RotationBasis3 = Vector3::UnitZ;
	Matrix3x3 Rotation(RotationBasis1, RotationBasis2, RotationBasis3);

	// FinalTransform
	Matrix3x3 FinalTransform = Translation * Rotation;


	for (auto v : Square)
	{
		Vector3 newV(v, 1);
		Vector3 finalV = FinalTransform * newV;

		r.DrawPoint(finalV.ToVector2(), TetrominoColor[0]);
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
