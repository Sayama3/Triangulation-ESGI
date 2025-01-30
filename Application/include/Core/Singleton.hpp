//
// Created by ianpo on 30/01/2025.
//

#pragma once

namespace TRG::Application {
	template<class Self>
	class Singleton {
	public:
		static bool HasInstance();
		static Self* TryGet();
		static Self& Get();
	public:
		static std::unique_ptr<Self> Unregister();
	public:
		static void Register(std::unique_ptr<Self> self);
		static std::unique_ptr<Self> TryRegister(std::unique_ptr<Self> self);
		template<typename ... Args>
		static void Register(Args&& ... args);
		template<typename ... Args>
		static bool TryRegister(Args&& ... args);
	private:
		static inline std::unique_ptr<Self> s_Instance = nullptr;
	};

	template<class Self>
	bool Singleton<Self>::HasInstance() {
		return s_Instance;
	}

	template<class Self>
	Self* Singleton<Self>::TryGet() {
		if (s_Instance) return s_Instance.get();
		return nullptr;
	}

	template<class Self>
	Self & Singleton<Self>::Get() {
		return *s_Instance;
	}

	template<class Self>
	std::unique_ptr<Self> Singleton<Self>::Unregister() {
		return std::move(s_Instance);
	}

	template<class Self>
	void Singleton<Self>::Register(std::unique_ptr<Self> self) {
		s_Instance = self;
	}

	template<class Self>
	std::unique_ptr<Self> Singleton<Self>::TryRegister(std::unique_ptr<Self> self) {
		if (s_Instance) return std::move(self);
		Register(self);
		return nullptr;
	}

	template<class Self>
	template<typename ... Args>
	void Singleton<Self>::Register(Args &&...args) {
		s_Instance = std::make_unique<Self>(std::forward<Args>(args)...);
	}

	template<class Self>
	template<typename ... Args>
	bool Singleton<Self>::TryRegister(Args &&...args) {
		if (s_Instance) return false;
		Register(std::forward<Args>(args)...);
		return true;
	}
} // TRG::Application
