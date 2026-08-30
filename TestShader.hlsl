cbuffer ConstantBuffer : register(b0) {
float4x4 pvwMatrix;
float4x4 worldMatrix;
float4 cameraPosition; // xyz = camera position in world space
};

struct VertexInput {
	float3 position : POSITION;
};

struct PixelInput {
	float4 position      : SV_POSITION;
	float3 worldPosition : POSITION0;
};
/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////
// Vertex Shader
/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////
PixelInput vMain(VertexInput input) {
	PixelInput output;

	float4 localPosition = float4(input.position, 1.0f);
	float4 worldPosition = mul(worldMatrix, localPosition);

	output.position      = mul(pvwMatrix, localPosition);
	output.worldPosition = worldPosition.xyz;

	return output;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////
// Pixel Shader
/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////
float4 pMain(PixelInput input, bool isFrontFace : SV_IsFrontFace) : SV_Target {
	float3 positionDx = ddx(input.worldPosition);
	float3 positionDy = ddy(input.worldPosition);
	float3 normal = normalize(cross(positionDy, positionDx));

	if (!isFrontFace) normal = -normal;

	// Direction from the surface toward the camera.
	float3 lightDirection = normalize(cameraPosition.xyz - input.worldPosition);

	float diffuseStrength =	saturate(dot(normal, lightDirection));

	float3 objectColor = float3(0.2f, 0.6f, 1.0f);
	float ambient = 0.1f;

	float3 finalColor =	objectColor * (ambient + (1.0f - ambient) * diffuseStrength);

	return float4(finalColor, 1.0f);
}
