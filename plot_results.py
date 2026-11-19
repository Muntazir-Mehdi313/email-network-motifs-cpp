# Purpose: Creates a cleaner and more professional grouped bar chart of department triad Z-scores.

import os
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Load results
data = pd.read_csv("results/z_scores.csv")

data["Department_A_Z"] = pd.to_numeric(data["Department_A_Z"], errors="coerce")
data["Department_B_Z"] = pd.to_numeric(data["Department_B_Z"], errors="coerce")

data = data.dropna(subset=["Department_A_Z", "Department_B_Z"])

classes = data["Class"].astype(str)
x = np.arange(len(classes))
width = 0.34

dept_a = "Department 16"
dept_b = "Department 20"

values_a = data["Department_A_Z"].values
values_b = data["Department_B_Z"].values

# Style
plt.rcParams.update({
    "font.family": "DejaVu Sans",
    "font.size": 10,
    "axes.titlesize": 18,
    "axes.labelsize": 12,
    "xtick.labelsize": 10,
    "ytick.labelsize": 10,
    "legend.fontsize": 11
})

fig, ax = plt.subplots(figsize=(16, 8.5))
fig.patch.set_facecolor("#F5F7FA")
ax.set_facecolor("white")

# Create more top space so title and legend do not overlap
fig.subplots_adjust(top=0.80, bottom=0.14, left=0.08, right=0.97)

# Colors
color_a = "#173F6B"
color_b = "#1FA7A1"
threshold_color = "#D97706"

# Non-significant region
ax.axhspan(-2, 2, color="#EEF3F8", alpha=0.85, zorder=0)

# Threshold lines
ax.axhline(2, color=threshold_color, linestyle="--", linewidth=2, zorder=1)
ax.axhline(-2, color=threshold_color, linestyle="--", linewidth=2, zorder=1)
ax.axhline(0, color="#64748B", linewidth=1.1, zorder=1)

# Bars
bars_a = ax.bar(
    x - width / 2,
    values_a,
    width,
    label=dept_a,
    color=color_a,
    edgecolor="white",
    linewidth=0.8,
    zorder=3
)

bars_b = ax.bar(
    x + width / 2,
    values_b,
    width,
    label=dept_b,
    color=color_b,
    edgecolor="white",
    linewidth=0.8,
    zorder=3
)

# Smarter labels
def add_labels(bars, values, side):
    for bar, value in zip(bars, values):
        xpos = bar.get_x() + bar.get_width() / 2

        # horizontal offset so both bar labels don't clash
        if side == "left":
            xoff = -6
        else:
            xoff = 6

        # vertical offset
        if value >= 0:
            yoff = 6
            va = "bottom"
        else:
            yoff = -6
            va = "top"

        ax.annotate(
            f"{value:.2f}",
            (xpos, value),
            textcoords="offset points",
            xytext=(xoff, yoff),
            ha="center",
            va=va,
            fontsize=8.5,
            color="#334155"
        )

add_labels(bars_a, values_a, "left")
add_labels(bars_b, values_b, "right")

# Title and subtitle
ax.set_title(
    "DIRECTED TRIAD SIGNIFICANCE ANALYSIS",
    loc="left",
    pad=36,
    fontweight="bold",
    color="#173F6B"
)

ax.text(
    0.0,
    1.02,
    "Comparison of observed email-network structures against randomized networks",
    transform=ax.transAxes,
    fontsize=11,
    color="#64748B",
    va="bottom"
)

# Axis labels
ax.set_xlabel("Directed Triad Class", fontweight="bold", color="#334155", labelpad=18)
ax.set_ylabel("Z-Score", fontweight="bold", color="#334155", labelpad=12)

ax.set_xticks(x)
ax.set_xticklabels(classes)

# Limits
ymin = min(-2.5, min(values_a.min(), values_b.min()) - 0.4)
ymax = max(2.5, max(values_a.max(), values_b.max()) + 0.5)
ax.set_ylim(ymin, ymax)

# Grid
ax.grid(axis="y", linestyle=":", linewidth=0.8, color="#CBD5E1", alpha=0.8)
ax.set_axisbelow(True)

# Clean spines
ax.spines["top"].set_visible(False)
ax.spines["right"].set_visible(False)
ax.spines["left"].set_color("#CBD5E1")
ax.spines["bottom"].set_color("#CBD5E1")

# Legend only for departments
ax.legend(
    loc="upper center",
    bbox_to_anchor=(0.68, 1.14),
    ncol=2,
    frameon=False
)

# Direct labels for threshold lines (instead of putting them in legend)
ax.text(
    len(classes) - 0.15,
    2.03,
    "+2 threshold",
    color=threshold_color,
    fontsize=10,
    ha="right",
    va="bottom",
    fontweight="bold"
)

ax.text(
    len(classes) - 0.15,
    -2.03,
    "-2 threshold",
    color=threshold_color,
    fontsize=10,
    ha="right",
    va="top",
    fontweight="bold"
)

# Footer notes
fig.text(
    0.08,
    0.05,
    "Interpretation: Values beyond +2 or -2 indicate unusually high or low triad counts.",
    fontsize=9,
    color="#475569"
)

fig.text(
    0.08,
    0.025,
    "Source: Email-Eu-core dataset | 48 randomized networks per department | R = 5298",
    fontsize=8.5,
    color="#64748B"
)

# Save outputs
os.makedirs("results", exist_ok=True)

plt.savefig("results/triad_z_scores.png", dpi=300, bbox_inches="tight")
plt.savefig("results/triad_z_scores.pdf", bbox_inches="tight")

print("Chart successfully generated!")
print("PNG: results/triad_z_scores.png")
print("PDF: results/triad_z_scores.pdf")

plt.show()