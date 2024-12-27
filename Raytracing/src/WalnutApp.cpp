#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override // Wird jeden Frame gerufen
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last Render: %.3fms", m_LastRenderTime);
		if(ImGui::Button("Render")) // Auf Button klick wird Render gemacht
		{
			Render();
		}
		ImGui::End();
		// Entfernt das Padding
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)); 

		// Button der Hauptfunktionen
		ImGui::Begin("Viewport");
		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;
		if (m_Image) {
			ImGui::Image(m_Image->GetDescriptorSet(), { (float)m_Image->GetWidth(),(float)m_Image->GetHeight() });
		}
		// Das Ende vom Erstellen der Buttons 
		ImGui::End();
		ImGui::PopStyleVar();

		Render();  // Wird dauerhaft aufgerufen
	}

	void Render() {

		Timer timer;

		// Wenn das Bild nicht vorhanden ist 
		// oder das Bild von der Gr��e oder  der H�he ver�ndert
		//  mache folgendens: 
		if (!m_Image || m_ViewportWidth !=m_Image->GetWidth() || m_ViewportHeight != m_Image->GetHeight()) {
			m_Image = std::make_shared<Image>(m_ViewportWidth, m_ViewportHeight, ImageFormat::RGBA); 
			delete[] m_ImageData; // ImageBuffer l�schen 
			// ImageBuffer neu belegen
			// Mal 4 ist unn�tig (da RGBA 4 bite ben�tigt) da uint32_t 4 Bytes gro� ist 
			m_ImageData = new uint32_t[m_ViewportWidth * m_ViewportHeight];
		}

		for (uint32_t i = 0; i < m_ViewportWidth * m_ViewportHeight; i++){
			m_ImageData[i] =Random::UInt(); // Magenta Pixel werden eingef�hrt (RBGA)
			m_ImageData[i] |= 0xff000000;
		}
		m_Image->SetData(m_ImageData); // Auf die GPU laden
		m_LastRenderTime = timer.ElapsedMillis();
	}

private:
	/// Zum definieren der Wert (Attribute)
	std::shared_ptr<Image> m_Image;
	uint32_t* m_ImageData = nullptr;
	uint32_t m_ViewportWidth = 0;
	uint32_t m_ViewportHeight = 0;
	float m_LastRenderTime;
};



Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Raytracing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}