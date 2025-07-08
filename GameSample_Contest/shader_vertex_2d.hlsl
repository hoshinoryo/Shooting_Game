/*==============================================================================

   2D�`��p���_�V�F�[�_�[ [shader_vertex_2d.hlsl]
														 Author : Youhei Sato
														 Date   : 2025/05/15
--------------------------------------------------------------------------------

==============================================================================*/

// �萔�o�b�t�@

// ���e�ϊ��s��
cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    float4x4 proj;
};

// ���[���h���W�ϊ��s��
cbuffer VS_CONSTANT_BUFFER : register(b1)
{
    float4x4 world;
};

// ���_�V�F�[�_�[�̓��͍\���́iCPU���̒��_�o�b�t�@����󂯎��j
struct VS_IN
{
    float4 posL  : POSITION0;
    float4 color : COLOR0;
    float2 uv    : TEXCOORD0;
};

// ���_�V�F�[�_�[�̏o�͍\���́i���̃X�e�[�W�ɓn���f�[�^�j
struct VS_OUT
{
    float4 posH  : SV_Position; // �V�X�e����`�̒��_�ʒu�i�N���b�v��ԍ��W�j
    float4 color : COLOR0;
    float2 uv    : TEXCOORD0;
};


//=============================================================================
// ���_�V�F�[�_
//=============================================================================
VS_OUT main(VS_IN vi)
{
    VS_OUT vo;
    
    // ���W�ϊ�
    float4x4 mtx = mul(world, proj); // ���[���h���W�n�Ɠ��e�ϊ����W�n�̊|���Z �� �ϊ��}�g���N�X
    
    // vi.posL = mul(vi.posL, world);
    vo.posH = mul(vi.posL, mtx); // ���[�J�����W�ƕϊ����W�̊|���Z
    
    vo.color = vi.color;
    vo.uv = vi.uv;
    
	return vo;
}


//    CPU���̒��_�o�b�t�@�i�ʒu + �F + UV�j
//    ��
// �� ���_�V�F�[�_�[�֐��i VS_IN �� VS_OUT�j
//    ��
// �� ���X�^���C�U�i�ʒu�͉�ʍ��W�ɕϊ��A �F��UV�͕�Ԃ����j
//    ��
// �� �s�N�Z���V�F�[�_�[
