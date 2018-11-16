// ���[�g�V�O�l�`���̐錾
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                    "DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "StaticSampler(s0, "\
                                  "filter = FILTER_MIN_MAG_MIP_LINEAR, "\
                                  "addressU = TEXTURE_ADDRESS_WRAP, "\
                                  "addressV = TEXTURE_ADDRESS_WRAP, "\
                                  "addressW = TEXTURE_ADDRESS_WRAP, "\
                                  "mipLodBias = 0.0f, "\
                                  "maxAnisotropy = 0, "\
                                  "comparisonFunc = COMPARISON_NEVER, "\
                                  "borderColor = STATIC_BORDER_COLOR_TRANSPARENT_BLACK, "\
                                  "minLOD = 0.0f, "\
                                  "maxLOD = 3.402823466e+38f, "\
                                  "space = 0, "\
                                  "visibility = SHADER_VISIBILITY_ALL)"

// �g�`�f�[�^
float wave[2940] : register(b0);

// ����
RWStructuredBuffer<float> real : register(u0);
// ����
//RWStructuredBuffer<float> imag : register(u1);

// �~����
#define PI 3.14159265f

// �f�[�^�T�C�Y
#define SIZE 2940

// �n�j���O���֐�
float Hanning(uint i)
{
    return (SIZE % 2 == 0) ?
    //����
    0.5f - 0.5f * cos(2.0f * PI * i / SIZE) :
    //�
    0.5f - 0.5f * cos(2.0f * PI * (i + 0.5f) / SIZE);
}

[numthreads(size, size, 1)]
void CS(uint3 id : SV_DispatchThreadID)
{
    float hanning = Hanning(id.x);
    float tmp = wave[id.y] * hanning;

    float r = cos(2.0f * PI * id.x * id.y / SIZE);
    float i = -sin(2.0f * PI * id.x * id.y / SIZE);

    real[id.x] += r * tmp - i * 0.0f;
    //imag[id.x] += r * 0.0f + i * tmp;
}