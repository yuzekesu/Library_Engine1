#pragma once
#include "Timer.h"
#include <condition_variable>
#include <d3d11.h>
#include <dxgi1_6.h>
#include <mutex>
#include <queue>
#include <thread>
#include <wrl.h>
using Microsoft::WRL::ComPtr;

namespace Render {
	class CommandExecutor {
		struct Submission {
			ComPtr<ID3D11CommandList> pCommandList;
			ComPtr<IDXGISwapChain4> pSwapChain;
		};
	public:
		CommandExecutor(ID3D11DeviceContext& immediate);
		void Submit(ComPtr<ID3D11CommandList>, ComPtr<IDXGISwapChain4>);
	private:
		ID3D11DeviceContext& _immediate;
		std::mutex _renderQueueMutex;
		std::condition_variable_any _renderQueueChanged;
		std::queue<Render::CommandExecutor::Submission> _renderQueue;
		std::jthread _renderThread; // make sure is last === destruct first.
	};
}

