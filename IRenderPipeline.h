#pragma once
#include "DirectX11Manager.h"
#include "IRenderWindow.h"

/// # ABSTRACT
/// 1. Represent rendering pipeline.
/// # REMARK
/// 1. A DisplayManager can own multiple IRenderPipeline objects.
class IRenderPipeline {
public:
	virtual ~IRenderPipeline() = default;
	virtual void Run(IRenderWindow&, DirectX11Manager&) = 0;
protected:
	IRenderPipeline() = default;
};

