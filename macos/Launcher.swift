import AppKit
import SwiftUI

// Render the launcher's own vector mark while building the app bundle.
func writeIcons(to directory: String) {
    for pixels in [16, 32, 64, 128, 256, 512, 1024] {
        guard let bitmap = NSBitmapImageRep(bitmapDataPlanes: nil, pixelsWide: pixels, pixelsHigh: pixels,
                                           bitsPerSample: 8, samplesPerPixel: 4, hasAlpha: true,
                                           isPlanar: false, colorSpaceName: .deviceRGB,
                                           bytesPerRow: 0, bitsPerPixel: 0) else { continue }
        NSGraphicsContext.saveGraphicsState()
        NSGraphicsContext.current = NSGraphicsContext(bitmapImageRep: bitmap)
        let transform = NSAffineTransform()
        transform.scale(by: CGFloat(pixels) / 1024)
        transform.concat()
        NSColor(red: 0.04, green: 0.09, blue: 0.12, alpha: 1).setFill()
        NSBezierPath(roundedRect: NSRect(x: 28, y: 28, width: 968, height: 968), xRadius: 190, yRadius: 190).fill()
        let ink = NSColor(red: 0.82, green: 0.66, blue: 0.31, alpha: 1)
        ink.setStroke()
        let border = NSBezierPath(roundedRect: NSRect(x: 102, y: 102, width: 820, height: 820), xRadius: 114, yRadius: 114)
        border.lineWidth = 13
        border.stroke()
        let circle = NSBezierPath(ovalIn: NSRect(x: 204, y: 204, width: 616, height: 616))
        circle.lineWidth = 6
        circle.stroke()
        let letter = NSAttributedString(string: "L", attributes: [
            .font: NSFont(name: "Copperplate", size: 480) ?? NSFont.systemFont(ofSize: 480),
            .foregroundColor: ink])
        let size = letter.size()
        letter.draw(at: NSPoint(x: (1024 - size.width) / 2, y: (1024 - size.height) / 2 + 25))
        NSGraphicsContext.restoreGraphicsState()
        if let png = bitmap.representation(using: .png, properties: [:]) {
            let url = URL(fileURLWithPath: directory)
            if [16, 32, 128, 256, 512].contains(pixels) {
                try? png.write(to: url.appendingPathComponent("icon_\(pixels)x\(pixels).png"))
            }
            if [32, 64, 256, 512, 1024].contains(pixels) {
                try? png.write(to: url.appendingPathComponent("icon_\(pixels / 2)x\(pixels / 2)@2x.png"))
            }
        }
    }
}

// Downsample the full-resolution originals once per display scale. This avoids
// thin wordmark strokes aliasing when SwiftUI repeatedly shrinks a large texture.
func refinedLogo(_ original: NSImage, width: CGFloat) -> NSImage {
    let target = NSSize(width: width, height: 20)
    let image = NSImage(size: target)
    let ratio = min(target.width / original.size.width, target.height / original.size.height)
    let fitted = NSSize(width: original.size.width * ratio, height: original.size.height * ratio)
    for scale in [1, 2, 3] {
        guard let bitmap = NSBitmapImageRep(bitmapDataPlanes: nil,
            pixelsWide: Int(target.width) * scale, pixelsHigh: Int(target.height) * scale,
            bitsPerSample: 8, samplesPerPixel: 4, hasAlpha: true, isPlanar: false,
            colorSpaceName: .deviceRGB, bytesPerRow: 0, bitsPerPixel: 0) else { continue }
        NSGraphicsContext.saveGraphicsState()
        NSGraphicsContext.current = NSGraphicsContext(bitmapImageRep: bitmap)
        NSGraphicsContext.current?.imageInterpolation = .high
        original.draw(in: NSRect(x: (target.width - fitted.width) * CGFloat(scale) / 2,
                                 y: (target.height - fitted.height) * CGFloat(scale) / 2,
                                 width: fitted.width * CGFloat(scale), height: fitted.height * CGFloat(scale)),
                      from: .zero, operation: .copy, fraction: 1)
        NSGraphicsContext.restoreGraphicsState()
        bitmap.size = target
        image.addRepresentation(bitmap)
    }
    return image
}

private let gold = Color(red: 0.82, green: 0.66, blue: 0.31)
private let parchment = Color(red: 0.94, green: 0.91, blue: 0.81)
private let muted = Color(red: 0.66, green: 0.70, blue: 0.70)
private let navy = Color(red: 0.035, green: 0.075, blue: 0.10)

struct Check: Identifiable, Decodable {
    let id: String
    let title: String
    let state: String
    let detail: String
}

struct Report: Decodable {
    let app: String
    let version: String
    let crossplay: Bool
    let checks: [Check]
    let ready: Bool
    let repairable: Bool
    let running: Bool
    let launched: Bool?
    let message: String?
    let running_message: String?
    let steam_session: SteamSession?
    let lekmod_installed: Bool?
    let lekmap_installed: Bool?
}

struct SteamSession: Decodable {
    let state: String
    let label: String
}

final class LauncherModel: ObservableObject {
    @Published var report: Report?
    @Published var busy = false
    @Published var activity = "Locating Civilization V…"
    @Published var error: String?
    @Published var app = ""
    @Published var crossplay = false
    @Published var artwork: NSImage?
    @Published var logos: [String: NSImage] = [:]
    @Published var log = ""
    @Published var showLog = false
    private var process: Process?
    private var buffer = Data()
    private var gotResult = false
    private var probingGame = false
    private var launchRequestedAt: Date?

    var gameActive: Bool { report?.running == true || report?.launched == true }

    var title: String {
        if busy { return "Preparing your game" }
        if error != nil { return "Needs attention" }
        guard let report else { return "Welcome to Lekmod" }
        if report.running { return "Civilization V is running" }
        if report.launched == true { return "Opening Civilization V" }
        if report.ready { return "Ready to play" }
        if report.app.isEmpty { return "Choose your game" }
        if report.lekmod_installed == false { return "Lekmod not installed" }
        return report.repairable ? "Installation needs repair" : "Needs attention"
    }
    var subtitle: String {
        if busy { return activity }
        if let error { return error }
        guard let report else { return "Select your Steam installation to get started." }
        if report.running { return "Close the game before repairing or changing cross-play." }
        if report.launched == true { return "Steam has received the launch request." }
        if report.ready { return "Your installation has passed all launch checks." }
        if let message = report.message { return message }
        return report.repairable
            ? "Restore the files below, then continue through Steam."
            : "Resolve the items below, then check your installation again."
    }
    var buttonTitle: String {
        if busy { return "Please wait…" }
        if report?.launched == true { return "Opening Civilization V…" }
        if report?.running == true { return "Game is running" }
        if report?.ready == true { return "Play" }
        if report?.repairable == true { return report?.lekmod_installed == false ? "Install & Play" : "Repair & Play" }
        return app.isEmpty ? "Choose Civilization V" : "Check Again"
    }

    func append(_ text: String) {
        log += text + "\n"
        if log.count > 80000 { log = String(log.suffix(60000)) }
    }

    func receive(_ data: Data) {
        buffer.append(data)
        while let newline = buffer.firstIndex(of: 10) {
            let line = buffer.prefix(upTo: newline)
            buffer.removeSubrange(...newline)
            guard !line.isEmpty else { continue }
            guard let event = try? JSONSerialization.jsonObject(with: line) as? [String: Any],
                  let kind = event["event"] as? String else {
                append(String(decoding: line, as: UTF8.self))
                continue
            }
            switch kind {
            case "selection":
                app = event["app"] as? String ?? app
                crossplay = event["crossplay"] as? Bool ?? crossplay
            case "artwork":
                if let path = event["background"] as? String { artwork = NSImage(contentsOfFile: path) }
            case "logos":
                for key in ["civ", "bnw", "lekmod"] {
                    if let path = event[key] as? String, let image = NSImage(contentsOfFile: path) { logos[key] = key == "civ" ? image : refinedLogo(image, width: key == "bnw" ? 64 : 56) }
                }
            case "progress":
                activity = event["message"] as? String ?? activity
                append(activity)
            case "error":
                error = event["message"] as? String ?? "An unexpected error occurred."
                append(error!)
            case "result":
                do {
                    report = try JSONDecoder().decode(Report.self, from: line)
                    app = report!.app
                    crossplay = report!.crossplay
                    gotResult = true
                    if report?.launched == true { launchRequestedAt = Date() }
                    for check in report!.checks {
                        append("[\(check.state)] \(check.title): \(check.detail)")
                    }
                } catch { self.error = "Could not read the installation report: \(error.localizedDescription)" }
            default: break
            }
        }
    }

    func run(_ action: String, preference: Bool? = nil) {
        guard !busy else { return }
        guard let repository = Bundle.main.object(forInfoDictionaryKey: "LekmodRepository") as? String,
              let python = Bundle.main.object(forInfoDictionaryKey: "LekmodPython") as? String else {
            error = "Launcher configuration is missing. Reopen Lekmod Launcher.command from your checkout."
            return
        }
        busy = true
        error = nil
        gotResult = false
        buffer = Data()
        activity = action == "status" ? "Checking your installation…" : "Validating before continuing…"
        append("\n\(Date().formatted()) — \(action)")
        let task = Process()
        task.executableURL = URL(fileURLWithPath: python)
        task.arguments = [repository + "/macos/launcher.py", action]
        if !app.isEmpty { task.arguments! += ["--app", app] }
        if let preference { task.arguments! += ["--crossplay", preference ? "on" : "off"] }
        task.currentDirectoryURL = URL(fileURLWithPath: repository)
        var environment = ProcessInfo.processInfo.environment
        environment["PYTHONUNBUFFERED"] = "1"
        environment["PATH"] = "/opt/homebrew/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin"
        task.environment = environment
        let pipe = Pipe()
        task.standardOutput = pipe
        task.standardError = pipe
        process = task
        // One reader preserves stdout ordering, drains the pipe, and keeps the UI responsive.
        DispatchQueue.global(qos: .userInitiated).async {
            do {
                try task.run()
                while true {
                    let chunk = pipe.fileHandleForReading.availableData
                    if chunk.isEmpty { break }
                    DispatchQueue.main.async { self.receive(chunk) }
                }
                task.waitUntilExit()
                DispatchQueue.main.async {
                    if !self.buffer.isEmpty { self.receive(Data([10])) }
                    self.busy = false
                    self.process = nil
                    if self.error == nil && (task.terminationStatus != 0 || !self.gotResult) {
                        self.error = "The launcher could not finish. Open Activity for details, then check again."
                    }
                }
            } catch {
                DispatchQueue.main.async {
                    self.busy = false
                    self.process = nil
                    self.error = "Could not start the launcher service: \(error.localizedDescription)"
                }
            }
        }
    }

    // Only process discovery runs periodically. Full file validation runs when
    // the observed game lifecycle changes, including games opened outside here.
    func refreshGameLifecycle() {
        guard !busy, !probingGame, report != nil, !app.isEmpty,
              let repository = Bundle.main.object(forInfoDictionaryKey: "LekmodRepository") as? String,
              let python = Bundle.main.object(forInfoDictionaryKey: "LekmodPython") as? String else { return }
        probingGame = true
        DispatchQueue.global(qos: .utility).async {
            let probe = Process()
            probe.executableURL = URL(fileURLWithPath: python)
            probe.arguments = [repository + "/macos/launcher.py", "process-status"]
            let pipe = Pipe()
            probe.standardOutput = pipe
            probe.standardError = FileHandle.nullDevice
            var running: Bool?
            do {
                try probe.run()
                let data = pipe.fileHandleForReading.readDataToEndOfFile()
                probe.waitUntilExit()
                if probe.terminationStatus == 0,
                   let event = try JSONSerialization.jsonObject(with: data) as? [String: Any] {
                    running = event["running"] as? Bool
                }
            } catch { /* A failed probe must not claim that the game has closed. */ }
            let observed = running
            DispatchQueue.main.async {
                self.probingGame = false
                guard !self.busy, let observed else { return }
                // Give Steam time to create its process after accepting the URL.
                if self.report?.launched == true, !observed,
                   let requested = self.launchRequestedAt, Date().timeIntervalSince(requested) < 15 { return }
                if observed != self.gameActive || self.report?.launched == true && observed {
                    self.run("status")
                }
            }
        }
    }

    func chooseGame() {
        let panel = NSOpenPanel()
        panel.title = "Choose Civilization V"
        panel.message = "Select Civilization V.app or its containing folder in your Steam library."
        panel.canChooseFiles = true
        panel.canChooseDirectories = true
        panel.allowsMultipleSelection = false
        panel.treatsFilePackagesAsDirectories = false
        panel.prompt = "Use This Game"
        if !app.isEmpty { panel.directoryURL = URL(fileURLWithPath: app).deletingLastPathComponent() }
        if panel.runModal() == .OK, let url = panel.url {
            app = url.path
            report = nil
            artwork = nil
            run("status")
        }
    }

    func primary() {
        if app.isEmpty { chooseGame() }
        else if report?.repairable == true { run("launch", preference: crossplay) }
        else { run("status") }
    }

    func openSteam() {
        if let url = NSWorkspace.shared.urlForApplication(withBundleIdentifier: "com.valvesoftware.steam") {
            NSWorkspace.shared.open(url)
        } else {
            error = "Steam could not be found. Install Steam, then check again."
        }
    }
}

struct DecoFrame: Shape {
    func path(in rect: CGRect) -> Path {
        let d: CGFloat = 10
        return Path { p in
            p.move(to: CGPoint(x: rect.minX + d, y: rect.minY))
            p.addLine(to: CGPoint(x: rect.maxX - d, y: rect.minY))
            p.addLine(to: CGPoint(x: rect.maxX, y: rect.minY + d))
            p.addLine(to: CGPoint(x: rect.maxX, y: rect.maxY - d))
            p.addLine(to: CGPoint(x: rect.maxX - d, y: rect.maxY))
            p.addLine(to: CGPoint(x: rect.minX + d, y: rect.maxY))
            p.addLine(to: CGPoint(x: rect.minX, y: rect.maxY - d))
            p.addLine(to: CGPoint(x: rect.minX, y: rect.minY + d))
            p.closeSubpath()
        }
    }
}

struct GoldButton: ButtonStyle {
    var primary = false
    func makeBody(configuration: Configuration) -> some View {
        GoldButtonContent(primary: primary, configuration: configuration)
    }
}

private struct GoldButtonContent: View {
    let primary: Bool
    let configuration: ButtonStyle.Configuration
    @State private var hovering = false
    @Environment(\.controlSize) private var controlSize
    @Environment(\.isEnabled) private var enabled
    var body: some View {
        configuration.label
            .font(.system(size: controlSize == .small ? 12 : 14, weight: .semibold, design: .serif))
            .tracking(0.6)
            .foregroundStyle(primary ? navy : parchment)
            .frame(maxWidth: .infinity)
            .padding(.vertical, controlSize == .small ? 8 : 14)
            .background(DecoFrame().fill(primary
                ? AnyShapeStyle(LinearGradient(colors: [Color(red: 0.895, green: 0.81, blue: 0.565), Color(red: 0.79, green: 0.675, blue: 0.395)], startPoint: .top, endPoint: .bottom))
                : AnyShapeStyle(LinearGradient(colors: [Color(red: 0.79, green: 0.73, blue: 0.53).opacity(0.16), navy.opacity(0.6)], startPoint: .top, endPoint: .bottom))))
            .overlay(DecoFrame().stroke(gold.opacity(primary ? 1 : 0.4), lineWidth: 1))
            .brightness(configuration.isPressed ? -0.1 : enabled && hovering ? 0.08 : 0)
            .shadow(color: gold.opacity(enabled && hovering ? 0.3 : 0), radius: 7)
            .contentShape(DecoFrame())
            .onHover { hovering = $0 }
            .animation(.easeOut(duration: 0.15), value: hovering)
            .opacity(enabled ? 1 : 0.5)
    }
}

struct FooterIconStyle: ButtonStyle {
    func makeBody(configuration: Configuration) -> some View {
        HoverIcon(configuration: configuration)
    }
    private struct HoverIcon: View {
        let configuration: ButtonStyle.Configuration
        @Environment(\.isEnabled) private var enabled
        @State private var hovering = false
        var body: some View {
            configuration.label
                .labelStyle(.iconOnly)
                .font(.system(size: 15, weight: .medium))
                .foregroundStyle(enabled && hovering ? parchment : gold)
                .frame(width: 40, height: 28)
                .background(RoundedRectangle(cornerRadius: 5)
                    .fill(gold.opacity(enabled && hovering ? 0.16 : 0)))
                .shadow(color: gold.opacity(enabled && hovering ? 0.45 : 0), radius: 5)
                .brightness(configuration.isPressed ? -0.12 : 0)
                .opacity(enabled ? 1 : 0.35)
                .contentShape(RoundedRectangle(cornerRadius: 5))
                .onHover { hovering = $0 }
                .animation(.easeOut(duration: 0.15), value: hovering)
        }
    }
}

struct Ornament: View {
    var body: some View {
        HStack(spacing: 10) {
            Rectangle().fill(gold.opacity(0.35)).frame(height: 1)
            Rectangle().fill(gold).frame(width: 5, height: 5).rotationEffect(.degrees(45))
            Rectangle().fill(gold.opacity(0.35)).frame(height: 1)
        }
        .accessibilityHidden(true)
    }
}

struct StatusSeal: View {
    let state: String
    var symbol: String { state == "ok" ? "checkmark" : "xmark" }
    var body: some View {
        Image(systemName: symbol).font(.system(size: 10, weight: .medium))
            .foregroundStyle(state == "blocked" ? Color(red: 0.86, green: 0.61, blue: 0.48) : gold)
            .frame(width: 16, height: 16)
            .accessibilityLabel(state == "ok" ? "Ready" : state == "blocked" ? "Needs attention" : "Pending")
    }
}

struct StatusBadge: View {
    let text: String
    let state: String
    var body: some View {
        Image(systemName: state == "ok" ? "checkmark" : "xmark")
            .font(.system(size: 13, weight: .medium))
            .foregroundStyle(gold)
            .frame(width: 48, height: 25)
            .help(text)
            .accessibilityLabel(text)
    }
}

// Compact vector brand silhouettes share the same Civ gold stroke as the controls.
struct BrandMark: Shape {
    let name: String
    func path(in rect: CGRect) -> Path {
        var p = Path()
        if name == "windows" {
            for (x0, x1) in [(0.08, 0.46), (0.55, 0.94)] {
                for lower in [false, true] {
                    let y0 = lower ? 0.55 : 0.18 - x0 * 0.12
                    let y1 = lower ? 0.82 + x0 * 0.12 : 0.45
                    p.move(to: CGPoint(x: x0, y: y0))
                    p.addLine(to: CGPoint(x: x1, y: lower ? 0.55 : 0.18 - x1 * 0.12))
                    p.addLine(to: CGPoint(x: x1, y: lower ? 0.82 + x1 * 0.12 : 0.45))
                    p.addLine(to: CGPoint(x: x0, y: y1)); p.closeSubpath()
                }
            }
        } else {
            p.addEllipse(in: CGRect(x: 0.53, y: 0.08, width: 0.4, height: 0.4))
            p.addEllipse(in: CGRect(x: 0.61, y: 0.16, width: 0.24, height: 0.24))
            p.addEllipse(in: CGRect(x: 0.22, y: 0.58, width: 0.3, height: 0.3))
            p.move(to: CGPoint(x: 0.32, y: 0.58)); p.addLine(to: CGPoint(x: 0.54, y: 0.25))
            p.move(to: CGPoint(x: 0.52, y: 0.73)); p.addLine(to: CGPoint(x: 0.83, y: 0.46))
            p.move(to: CGPoint(x: 0.02, y: 0.57)); p.addLine(to: CGPoint(x: 0.38, y: 0.71))
            p.move(to: CGPoint(x: 0.02, y: 0.7)); p.addLine(to: CGPoint(x: 0.32, y: 0.83))
        }
        return p.applying(CGAffineTransform(scaleX: rect.width, y: rect.height))
    }
}

struct LauncherView: View {
    @ObservedObject var model: LauncherModel

    private func summary(_ ids: [String]) -> (state: String, help: String) {
        let checks = (model.report?.checks ?? []).filter { ids.contains($0.id) }
        let state = checks.contains { $0.state == "blocked" } ? "blocked"
            : checks.contains { $0.state == "repair" } ? "repair"
            : checks.isEmpty ? "pending" : "ok"
        return (state, checks.map { $0.detail }.joined(separator: "\n"))
    }

    private func row(_ title: String, _ icon: String, _ ids: [String], good: String) -> some View {
        let value = summary(ids)
        let absent = (title == "Engine" || title == "Assets") && model.report?.lekmod_installed == false
            || title == "Map" && model.report?.lekmap_installed == false
        let text = absent ? "Not installed" : value.state == "ok" ? good : value.state == "blocked" ? "Needs attention"
            : value.state == "repair" ? "Repair needed" : "Not checked"
        return HStack(spacing: 11) {
            HStack(spacing: 9) {
                mark(icon).frame(width: 20)
                Text(title).font(.system(size: 13, design: .serif)).foregroundStyle(parchment)
                    .frame(width: 64, alignment: .center)
            }.frame(maxWidth: .infinity, alignment: .center)
            StatusBadge(text: text, state: absent ? "pending" : value.state)
        }
        .frame(maxHeight: .infinity)
        .help(value.help)
        .accessibilityElement(children: .combine)
    }

    var body: some View {
        GeometryReader { geometry in
            ZStack {
                navy
                if let image = model.artwork {
                    Image(nsImage: image).resizable().aspectRatio(contentMode: .fill)
                        .frame(width: geometry.size.width, height: geometry.size.height).clipped()
                        .accessibilityHidden(true)
                }
                LinearGradient(colors: [.black.opacity(0.06), navy.opacity(0.08), navy.opacity(0.76)],
                               startPoint: .top, endPoint: .bottom)
                HStack(alignment: .top, spacing: 32) {
                    VStack(alignment: .center, spacing: 0) {
                        Text("SID MEIER’S").font(.system(size: 9, weight: .medium, design: .serif)).tracking(3).foregroundStyle(parchment)
                        Text("CIVILIZATION V").font(.custom("Copperplate", size: 27)).foregroundStyle(gold)
                            .shadow(color: .black, radius: 5)
                        HStack(spacing: 11) {
                            Rectangle().fill(gold).frame(width: 24, height: 1)
                            Text("LEKMOD").font(.custom("Copperplate", size: 38)).tracking(3).foregroundStyle(parchment)
                            Rectangle().fill(gold).frame(width: 24, height: 1)
                        }.padding(.top, 3)
                        Text("MAC LAUNCHER").font(.system(size: 9, weight: .semibold, design: .serif)).tracking(2.5)
                            .foregroundStyle(gold.opacity(0.85)).padding(.top, 7)
                        Spacer(minLength: 24)
                        components
                        installation.padding(.top, 24)
                        Text(model.report?.version ?? "Local checkout")
                            .font(.system(size: 10, design: .serif)).foregroundStyle(muted).padding(.top, 16)
                    }.frame(width: (geometry.size.width - 60) / 2)
                    mainPanel.frame(width: (geometry.size.width - 132) / 2)
                }.padding(.horizontal, 32).padding(.top, 48).padding(.bottom, 32)
            }
        }
        .ignoresSafeArea()
        .preferredColorScheme(.dark).tint(gold).multilineTextAlignment(.center)
        .sheet(isPresented: $model.showLog) { activitySheet }
    }

    @ViewBuilder private func mark(_ name: String, size: CGFloat = 16) -> some View {
        if let image = model.logos[name] {
            Image(nsImage: image).resizable().interpolation(.high).aspectRatio(contentMode: .fit)
                .frame(width: name == "civ" ? size : name == "bnw" ? 64 : 56 * size / 20,
                       height: name == "civ" ? size : name == "bnw" ? 20 : size)
                .accessibilityHidden(true)
        } else if name == "steam" || name == "windows" {
            BrandMark(name: name).stroke(gold, style: StrokeStyle(lineWidth: 1.3, lineCap: .round, lineJoin: .round))
                .frame(width: size, height: size).accessibilityHidden(true)
        } else {
            Image(systemName: ["civ": "building.columns", "bnw": "globe", "lekmod": "l.square" ][name] ?? name)
                .font(.system(size: size, design: .serif)).foregroundStyle(gold).frame(width: size, height: size).accessibilityHidden(true)
        }
    }

    private var steamRow: some View {
        HStack(spacing: 11) {
            HStack(spacing: 9) {
                mark("steam").frame(width: 20)
                Text("Steam").font(.system(size: 13, design: .serif)).foregroundStyle(parchment)
                    .frame(width: 64, alignment: .center)
                    .overlay(alignment: .trailing) {
                        Button(action: model.openSteam) {
                            Image(systemName: "arrow.up.right.square").font(.system(size: 13))
                        }.buttonStyle(.plain).foregroundStyle(gold).help("Open Steam")
                            .accessibilityLabel("Open Steam").offset(x: 22)
                    }
            }.frame(maxWidth: .infinity, alignment: .center)
            StatusBadge(text: model.report?.steam_session?.label ?? "Checking",
                        state: model.report?.steam_session?.state ?? "pending")
        }.frame(maxHeight: .infinity)
    }

    private var crossplayRow: some View {
        HStack(spacing: 11) {
            HStack(spacing: 9) {
                mark("windows").frame(width: 20)
                Text("Crossplay").font(.system(size: 13, design: .serif)).foregroundStyle(parchment)
                    .frame(width: 64, alignment: .center)
            }.frame(maxWidth: .infinity, alignment: .center)
            Toggle("Crossplay", isOn: Binding(get: { model.crossplay }, set: {
                model.crossplay = $0
                model.run("status", preference: $0)
            })).labelsHidden().toggleStyle(.switch).controlSize(.small)
                .disabled(model.busy || model.app.isEmpty || model.gameActive)
                .frame(width: 48, alignment: .trailing)
                .overlay(alignment: .leading) {
                    StatusSeal(state: summary(["crossplay"]).state).offset(x: -22)
                }
                .accessibilityLabel("Enable Crossplay")
                .help("Remembered across Steam repairs. Changes apply before launch. Players need matching Lekmod and DLC.")
        }.padding(.vertical, 12)
    }

    private func component(_ title: String, _ key: String, installed: Bool?) -> some View {
        VStack(spacing: 6) {
            HStack(spacing: 11) {
                HStack(spacing: 9) {
                    mark(key == "lekmod" ? "lekmod" : "map", size: 14).frame(width: 40)
                    Text(title).font(.system(size: 14, design: .serif)).foregroundStyle(parchment)
                        .frame(width: 64, alignment: .center)
                }.frame(maxWidth: .infinity, alignment: .center)
                StatusBadge(text: installed == true ? "Installed" : installed == false ? "Not installed" : "Checking",
                            state: installed == true ? "ok" : "pending")
            }
            HStack(spacing: 12) {
                Button { model.run("install-" + key, preference: model.crossplay) } label: {
                    Label("Install", systemImage: "arrow.down.to.line")
                }.disabled(model.busy || model.report?.repairable != true || model.gameActive || installed == true)
                    .accessibilityLabel("Install " + title)
                Button { model.run("uninstall-" + key) } label: {
                    Label("Uninstall", systemImage: "minus.circle")
                }.disabled(model.busy || model.report?.repairable != true || model.gameActive || installed != true)
                    .accessibilityLabel("Uninstall " + title)
                    .help("Remove " + title + ". Keep a backup, saved games, and the other component.")
            }.buttonStyle(GoldButton(primary: false)).controlSize(.small).frame(maxWidth: 280)
        }
    }

    private var components: some View {
        VStack(spacing: 12) {
            component("Lekmod", "lekmod", installed: model.report?.lekmod_installed)
            Rectangle().fill(gold.opacity(0.28)).frame(height: 1)
            component("Lekmap", "lekmap", installed: model.report?.lekmap_installed)
        }
    }

    private var installation: some View {
        let status = summary(["steam", "game"])
        return VStack(alignment: .leading, spacing: 8) {
            HStack(spacing: 9) {
                Image(systemName: "externaldrive").foregroundStyle(gold)
                Text("Game location").font(.system(size: 11, weight: .medium, design: .serif)).foregroundStyle(parchment)
                StatusSeal(state: status.state)
                Spacer()
                Button("Change…", action: model.chooseGame).buttonStyle(.plain).foregroundStyle(gold)
                    .disabled(model.busy).font(.system(size: 11, design: .serif))
            }
            Text(model.app.isEmpty ? "Choose your Steam installation" : (model.app as NSString).abbreviatingWithTildeInPath)
                .font(.system(size: 10, design: .serif)).foregroundStyle(muted).lineLimit(1).truncationMode(.middle)
                .help(model.app + "\n" + status.help)
        }
    }

    private var mainPanel: some View {
        VStack(alignment: .leading, spacing: 0) {
            HStack(spacing: 10) {
                if model.busy {
                    ProgressView().controlSize(.small).scaleEffect(0.8).accessibilityLabel("Checking installation")
                }
                Text(model.title).font(.system(size: 23, weight: .regular, design: .serif))
                    .foregroundStyle(parchment).lineLimit(1).minimumScaleFactor(0.85)
            }.frame(maxWidth: .infinity, alignment: .center)
            Ornament().padding(.top, 18).padding(.bottom, 5)
            VStack(spacing: 0) {
                steamRow
                row("Game", "civ", ["game", "host"], good: "Compatible")
                row("DLC", "civ", ["dlc"], good: "Installed")
                row("Engine", "gearshape", ["core", "abi", "signature"], good: "Verified")
                row("Assets", "gearshape", ["lekmod_assets"], good: "Verified")
                row("Map", "map", ["lekmap_assets"], good: "Verified")
                row("Version", "arrow.triangle.2.circlepath", ["source"], good: "Up to date")
            }.frame(maxHeight: .infinity).padding(.vertical, 8)
            if model.busy || model.error != nil || model.report?.ready != true || model.report?.launched == true {
                let detail = model.busy || model.error != nil || model.report?.running == true || model.report?.launched == true
                    ? model.subtitle
                    : model.report?.checks.first(where: { $0.state != "ok" })?.detail ?? model.subtitle
                Text(detail).font(.system(size: 11, design: .serif)).foregroundStyle(muted)
                    .lineLimit(1).multilineTextAlignment(.center)
                    .frame(maxWidth: .infinity).frame(height: 16).help(detail)
            } else {
                Color.clear.frame(height: 16)
            }
            Rectangle().fill(gold.opacity(0.23)).frame(height: 1)
            crossplayRow
            Button(action: model.primary) {
                Label(model.buttonTitle, systemImage: "play.fill")
            }.buttonStyle(GoldButton(primary: true))
                .disabled(model.busy || model.gameActive)
                .keyboardShortcut(.defaultAction)
            HStack(spacing: 0) {
                Button { model.run("status") } label: { Label("Check", systemImage: "arrow.clockwise") }
                    .help("Check installation")
                    .disabled(model.busy || model.app.isEmpty)
                    .frame(maxWidth: .infinity)
                Button { model.run("repair", preference: model.crossplay) } label: { Label("Repair", systemImage: "wrench.and.screwdriver") }
                    .disabled(model.busy || model.report?.repairable != true || model.gameActive)
                    .help("Repair without launching the game")
                    .frame(maxWidth: .infinity)
                Button { model.showLog = true } label: { Label("Activity", systemImage: "text.alignleft") }
                    .help("Activity log")
                    .frame(maxWidth: .infinity)
            }.buttonStyle(FooterIconStyle()).padding(.top, 8)
        }.padding(24).frame(maxHeight: .infinity)
            .background(DecoFrame().fill(navy.opacity(0.75)))
            .overlay(DecoFrame().stroke(gold.opacity(0.6), lineWidth: 1))
            .overlay(DecoFrame().insetForDecoration())
    }

    private var activitySheet: some View {
        VStack(alignment: .leading, spacing: 16) {
            HStack {
                Text("Launcher activity").font(.title2).foregroundStyle(parchment)
                Spacer()
                Button("Done") { model.showLog = false }.keyboardShortcut(.cancelAction)
            }
            ScrollView {
                Text(model.log.isEmpty ? "No activity yet." : model.log)
                    .font(.system(size: 11, design: .monospaced)).textSelection(.enabled)
                    .frame(maxWidth: .infinity, alignment: .leading).padding(12)
            }.background(.black.opacity(0.3))
            HStack {
                Button("Copy Log") {
                    NSPasteboard.general.clearContents()
                    NSPasteboard.general.setString(model.log, forType: .string)
                }
                Spacer()
                Button("Open Settings Folder") {
                    NSWorkspace.shared.open(FileManager.default.homeDirectoryForCurrentUser
                        .appendingPathComponent("Library/Application Support/Lekmod Launcher"))
                }
            }
        }.padding(24).frame(width: 720, height: 470).background(navy)
    }
}

extension DecoFrame {
    func insetForDecoration() -> some View {
        DecoFrame().stroke(gold.opacity(0.13), lineWidth: 1).padding(5).allowsHitTesting(false)
    }
}

final class AppDelegate: NSObject, NSApplicationDelegate, NSWindowDelegate {
    let model = LauncherModel()
    var window: NSWindow!
    private var lifecycleTimer: Timer?
    func applicationDidFinishLaunching(_ notification: Notification) {
        let menu = NSMenu()
        let appItem = NSMenuItem()
        let appMenu = NSMenu()
        appMenu.addItem(withTitle: "Quit Lekmod Launcher", action: #selector(NSApplication.terminate(_:)), keyEquivalent: "q")
        appItem.submenu = appMenu
        menu.addItem(appItem)
        let editItem = NSMenuItem()
        let edit = NSMenu(title: "Edit")
        edit.addItem(withTitle: "Copy", action: #selector(NSText.copy(_:)), keyEquivalent: "c")
        edit.addItem(withTitle: "Select All", action: #selector(NSText.selectAll(_:)), keyEquivalent: "a")
        editItem.submenu = edit
        menu.addItem(editItem)
        NSApp.mainMenu = menu
        window = NSWindow(contentRect: NSRect(x: 0, y: 0, width: 740, height: 530),
                          styleMask: [.titled, .closable, .miniaturizable, .resizable, .fullSizeContentView],
                          backing: .buffered, defer: false)
        window.title = "Lekmod Launcher"
        window.titleVisibility = .hidden
        window.titlebarAppearsTransparent = true
        window.minSize = NSSize(width: 740, height: 530)
        window.isOpaque = false
        window.backgroundColor = .clear
        window.isMovableByWindowBackground = true
        window.appearance = NSAppearance(named: .darkAqua)
        window.contentView = NSHostingView(rootView: LauncherView(model: model))
        window.delegate = self
        window.center()
        window.makeKeyAndOrderFront(nil)
        NSApp.activate(ignoringOtherApps: true)
        model.run("status")
        lifecycleTimer = Timer.scheduledTimer(withTimeInterval: 3, repeats: true) { [weak self] _ in
            self?.model.refreshGameLifecycle()
        }
    }
    func applicationDidBecomeActive(_ notification: Notification) {
        model.refreshGameLifecycle()
    }
    func applicationShouldTerminateAfterLastWindowClosed(_ sender: NSApplication) -> Bool { true }
    func applicationShouldTerminate(_ sender: NSApplication) -> NSApplication.TerminateReply {
        if model.busy {
            let alert = NSAlert()
            alert.messageText = "The launcher is still working"
            alert.informativeText = "Let the current check or repair finish before closing the launcher."
            alert.addButton(withTitle: "Keep Open")
            alert.runModal()
            return .terminateCancel
        }
        return .terminateNow
    }
    func windowShouldClose(_ sender: NSWindow) -> Bool {
        applicationShouldTerminate(NSApp) == .terminateNow
    }
}

let application = NSApplication.shared
if CommandLine.arguments.count == 3 && CommandLine.arguments[1] == "--write-icons" {
    writeIcons(to: CommandLine.arguments[2])
    exit(0)
}
let delegate = AppDelegate()
application.setActivationPolicy(.regular)
application.delegate = delegate
application.run()
