import os
from pathlib import Path

include_path = Path("include")
src_path = Path("src")
output_file = "project_code_report.txt"

module_descriptions = {
    "managers": "Менеджеры, управляющие глобальным состоянием и системами.",
    "render": "Классы для управления рендерингом.",
    "object/material": "Графические ресурсы: шейдеры, материалы и текстуры.",
    "object/geometry": "Геометрия объектов: меши, примитивы, скелеты и анимации.",
    "object/transform": "Компоненты, отвечающие за трансформации объектов.",
    "object/component/template": "Шаблонные компоненты.",
    "object/component": "Компоненты сущностей.",
    "object/entities/decore": "Декоративные объекты сцены.",
    "object/entities/mobes": "Объекты передвижения и NPC.",
    "object/entities/playeble": "Игровые персонажи.",
    "object/entities": "Общие сущности и сцена.",
    "threads": "Пул потоков для параллельного сохранения скриншотов.",
}

def collect_files():
    code_map = {}
    for header in include_path.rglob("*.hpp"):
        relative = header.relative_to(include_path)
        module_key = get_module_key(str(relative).replace("\\", "/"))
        cpp_file = src_path / relative.with_suffix(".cpp")
        code_map.setdefault(module_key, []).append((header, cpp_file if cpp_file.exists() else None))
    return code_map

def get_module_key(path):
    for key in module_descriptions:
        if path.startswith(key):
            return key
    return "Прочее"

def read_file(file_path):
    try:
        with open(file_path, "r", encoding="utf-8") as f:
            return f.read()
    except Exception as e:
        return f"// Ошибка чтения файла: {e}"

def generate_report(code_map):
    with open(output_file, "w", encoding="utf-8") as out:
        for module, files in sorted(code_map.items()):
            for hpp, cpp in sorted(files):
                title = hpp.relative_to(include_path)
                out.write(f"\nФайл: include\{title}\n\n")
                out.write(read_file(hpp))
                out.write("\n\n")
                if cpp:
                    title_cpp = cpp.relative_to(src_path)
                    out.write(f"Реализация: src\{title_cpp}\n\n")
                    out.write(read_file(cpp))
                    out.write("\n\n")

        print(f"[✓] Сохранён отчёт: {output_file}")


if __name__ == "__main__":
    code_data = collect_files()
    generate_report(code_data)
