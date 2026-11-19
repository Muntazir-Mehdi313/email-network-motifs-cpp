# Purpose: Creates a publication-ready grouped bar chart of departmental triad Z-scores.

import os
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Load the CSV exported by the C++ program.
data = pd.read_csv("results/z_scores.csv")

required_columns = {
    "Class", "Department_A_Z", "Department_B_Z",
    "Department_A_ID", "Department_B_ID", "R"
}
missing = required_columns.difference(data.columns)
if missing:
    raise ValueError(
        "The CSV is missing required columns: " + ", ".join(sorted(missing))
        + ". Recompile and run the updated main.cpp first."
    )

for column in ["Department_A_Z", "Department_B_Z"]:
    data[column] = pd.to_numeric(data[column], errors="coerce")

data = data.dropna(subset=["Department_A_Z", "Department_B_Z"])
if data.empty:
    raise ValueError("No valid Z-score rows were found in results/z_scores.csv.")

classes = data["Class"].astype(str).tolist()
x = np.arange(len(classes))
width = 0.34

r_value = int(data["R"].iloc[0])
dept_a = f"Department {int(data['Department_A_ID'].iloc[0])}"
dept_b = f"Department {int(data['Department_B_ID'].iloc[0])}"
values_a = data["Department_A_Z"].to_numpy()
values_b = data["Department_B_Z"].to_numpy()

# Set a consistent academic-report style.
plt.rcParams.update({
    "font.family": "DejaVu Sans",
    "font.size": 10,
    "axes.labelsize": 12,
    "xtick.labelsize": 10,
    "ytick.labelsize": 10,
    "legend.fontsize": 10,
    "pdf.fonttype": 42,
    "ps.fonttype": 42
})

fig, ax = plt.subplots(figsize=(15, 8))
fig.patch.set_facecolor("#F4F7FB")
ax.set_facecolor("#FFFFFF")
fig.subplots_adjust(left=0.09, right=0.97, top=0.78, bottom=0.19)

navy = "#173F6B"
teal = "#159E99"
orange = "#C77716"
slate = "#475569"

# Shade the conventional -2 to +2 reference band.
ax.axhspan(-2, 2, color="#EEF3F8", alpha=0.95, zorder=0)
ax.axhline(2, color=orange, linestyle="--", linewidth=1.8, zorder=2)
ax.axhline(-2, color=orange, linestyle="--", linewidth=1.8, zorder=2)
ax.axhline(0, color="#64748B", linewidth=1.0, zorder=2)

bars_a = ax.bar(
    x - width / 2, values_a, width,
    label=dept_a, color=navy,
    edgecolor="white", linewidth=0.8, zorder=3
)
bars_b = ax.bar(
    x + width / 2, values_b, width,
    label=dept_b, color=teal,
    edgecolor="white", linewidth=0.8, zorder=3
)

# Keep labels readable by positioning them relative to each bar.
def add_value_labels(bars, values, horizontal_offset):
    for bar, value in zip(bars, values):
        if abs(value) < 0.005:
            continue
        vertical_offset = 5 if value >= 0 else -5
        ax.annotate(
            f"{value:.2f}",
            (bar.get_x() + bar.get_width() / 2, value),
            xytext=(horizontal_offset, vertical_offset),
            textcoords="offset points",
            ha="center",
            va="bottom" if value >= 0 else "top",
            fontsize=8,
            color=slate,
            clip_on=False
        )

add_value_labels(bars_a, values_a, -2)
add_value_labels(bars_b, values_b, 2)

# Title and subtitle occupy separate figure-level positions.
fig.text(
    0.09, 0.94,
    f"DIRECTED TRIAD SIGNIFICANCE ANALYSIS  |  R = {r_value}",
    fontsize=17, fontweight="bold", color=navy, ha="left", va="top"
)
fig.text(
    0.09, 0.895,
    "Observed departmental triad counts compared with randomized networks",
    fontsize=10.5, color="#64748B", ha="left", va="top"
)

ax.set_xlabel("Directed Triad Class", fontweight="bold", color="#334155", labelpad=13)
ax.set_ylabel("Z-Score", fontweight="bold", color="#334155", labelpad=10)
ax.set_xticks(x)
ax.set_xticklabels(classes)

minimum = min(-2.45, float(min(values_a.min(), values_b.min())) - 0.35)
maximum = max(2.45, float(max(values_a.max(), values_b.max())) + 0.40)
ax.set_ylim(minimum, maximum)
ax.set_xlim(-0.65, len(classes) - 0.35)

# Put threshold descriptions inside the plot at the right edge.
left_x = -0.58
right_x = len(classes) - 0.42
ax.text(left_x, 2.04, "+2 reference", color=orange,
        fontsize=8.5, ha="left", va="bottom", fontweight="bold")
ax.text(right_x, -2.04, "-2 reference", color=orange,
        fontsize=8.5, ha="right", va="top", fontweight="bold")

ax.grid(axis="y", linestyle=":", linewidth=0.8, color="#CBD5E1", alpha=0.9)
ax.set_axisbelow(True)
ax.spines["top"].set_visible(False)
ax.spines["right"].set_visible(False)
ax.spines["left"].set_color("#CBD5E1")
ax.spines["bottom"].set_color("#CBD5E1")
ax.tick_params(axis="both", colors=slate, length=0, pad=7)

ax.legend(
    loc="upper center",
    bbox_to_anchor=(0.5, 1.12),
    ncol=2,
    frameon=False,
    handlelength=1.8,
    columnspacing=2.5
)

fig.text(
    0.09, 0.085,
    "Reference guide: Z-scores outside the ±2 band indicate unusually high or low counts under this comparison.",
    fontsize=8.5, color=slate, ha="left"
)
fig.text(
    0.09, 0.052,
    f"Email-Eu-core dataset  |  {dept_a} vs {dept_b}  |  48 randomized networks per department",
    fontsize=8, color="#64748B", ha="left"
)

os.makedirs("results", exist_ok=True)
plt.savefig("results/triad_z_scores.png", dpi=300, bbox_inches="tight")
plt.savefig("results/triad_z_scores.pdf", bbox_inches="tight")

print("Chart generated successfully.")
print("PNG: results/triad_z_scores.png")
print("PDF: results/triad_z_scores.pdf")
plt.show()
